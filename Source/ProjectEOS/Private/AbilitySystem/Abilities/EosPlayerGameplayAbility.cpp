// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/EosPlayerGameplayAbility.h"
#include "Character/PlayerCharacter/EosBasePlayerCharacter.h"
#include "PlayerController/EosBasePlayerController.h"
#include "Items/Weapon/EosBaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/EosAbilitySystemComponent.h"
#include "EosGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "EosFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameplayAbilityBlueprint.h"
#include "AbilitySystemBlueprintLibrary.h"

void UEosPlayerGameplayAbility::Client_ApplyDash_Implementation()
{
	ApplyDash();
}

void UEosPlayerGameplayAbility::Server_GetLastInputAndUpdateRotationForDash_Implementation(FVector InCachedLastDirection)
{
	CachedLastDirection = InCachedLastDirection;
	ApplyDash();
	Client_ApplyDash();
}

void UEosPlayerGameplayAbility::Server_GetLastInputAndUpdateRotationForAttack_Implementation(FVector InCachedLastDirection)
{
	// 워프 방향 수집
	CachedLastDirection = InCachedLastDirection;

	// 워프 키, 전방 로테이션 생성
	const FName WarpKey = FName(TEXT("AttackDirection"));
	FRotator RotFromX = UKismetMathLibrary::MakeRotFromX(InCachedLastDirection);

	// 서버 직접 실행
	GetPlayerCharacterFromActorInfo()->Server_AddOrUpdateWarpTargetfromLocationandRotation(WarpKey, FVector::ZeroVector, RotFromX);
}

void UEosPlayerGameplayAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEosPlayerGameplayAbility, CachedBasePlayerCharacter);
	DOREPLIFETIME(UEosPlayerGameplayAbility, CachedBasePlayerController);
	DOREPLIFETIME(UEosPlayerGameplayAbility, CachedLastDirection);
}

AEosBasePlayerCharacter* UEosPlayerGameplayAbility::GetPlayerCharacterFromActorInfo()
{
	if(!CachedBasePlayerCharacter.IsValid())
	{
		CachedBasePlayerCharacter = Cast<AEosBasePlayerCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedBasePlayerCharacter.IsValid() ? CachedBasePlayerCharacter.Get() : nullptr;
}

AEosBasePlayerController* UEosPlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!CachedBasePlayerController.IsValid())
	{
		CachedBasePlayerController = Cast<AEosBasePlayerController>(CurrentActorInfo->PlayerController);
	}
	return CachedBasePlayerController.IsValid() ? CachedBasePlayerController.Get() : nullptr;
}

UEosPlayerCombatComponent* UEosPlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}

// 플레이어 대미지 기반 스펙 핸들 제작
FGameplayEffectSpecHandle UEosPlayerGameplayAbility::MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{
	check(EffectClass);
	// 컨텍스트 핸들 생성
	FGameplayEffectContextHandle ContextHandle = GetEosAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	// 스펙 핸들 생성
	FGameplayEffectSpecHandle EffectSpecHandle = GetEosAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec
	(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	// SetByCaller로 태그와 대미지를 Map형태로 저장
	EffectSpecHandle.Data->SetSetByCallerMagnitude
	(
		EosGameplayTags::Common_SetByCaller_BaseDamage,
		InWeaponBaseDamage
	);

	// SetByCaller로 태그와 콤보 수를 Map형태로 저장
	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude
		(
			InCurrentAttackTypeTag,
			InUsedComboCount
		);
	}
	// 핸들 구조체를 반환
	return EffectSpecHandle;
}

FGameplayEffectSpecHandle UEosPlayerGameplayAbility::MakeSkillDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InSkillDamage, FGameplayTag InCurrentAttackTypeTag)
{
	check(EffectClass);

	// 컨텍스트 핸들 생성
	FGameplayEffectContextHandle ContextHandle = GetEosAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	// 이펙트 스펙핸들 생성
	FGameplayEffectSpecHandle EffectSpecHandle = GetEosAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec
	(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	// 태그와 대미지 매칭
	EffectSpecHandle.Data->SetSetByCallerMagnitude
	(
		InCurrentAttackTypeTag,
		InSkillDamage
	);

	// 이펙트 스펙핸들 반환
	return EffectSpecHandle;
}

void UEosPlayerGameplayAbility::ConsumeDirectionInput()
{
	// 리슨 여부 점검
	if(UEosFunctionLibrary::IsListenServer(GetWorld()))
	{
		// 워프키& 전방로테이션 생성
		const FName WarpKey = FName(TEXT("AttackDirection"));
		FRotator RotFromX = UKismetMathLibrary::MakeRotFromX(GetValidNormalizeInputVector());
		
		// 호스트 점검
		if (GetPlayerCharacterFromActorInfo()->HasAuthority())
		{
			if(IsLocallyControlled())
			{
				// 마지막 인풋 방향을 받아서 워프 실행
				CachedLastDirection = GetValidNormalizeInputVector();
				GetPlayerCharacterFromActorInfo()->Server_AddOrUpdateWarpTargetfromLocationandRotation(WarpKey, FVector::ZeroVector, RotFromX);
			}
		}
	}
	else
	{
		// 클라이언트 점검
		if (!GetPlayerCharacterFromActorInfo()->HasAuthority())
		{
			// 로테이션 요청
			Server_GetLastInputAndUpdateRotationForAttack(GetValidNormalizeInputVector());
		}
	}
}

void UEosPlayerGameplayAbility::ComputeDashDirectionAndDistance()
{
	// 리슨 여부 점검
	if (UEosFunctionLibrary::IsListenServer(GetWorld()))
	{
		// 호스트 점검
		if (GetPlayerCharacterFromActorInfo()->HasAuthority())
		{
			if (IsLocallyControlled())
			{
				// 마지막 인풋 방향을 받아놓고 대시 실행
				CachedLastDirection = GetValidNormalizeInputVector();
				ApplyDash();
			}
		}
	}
	else
	{
		if (!GetPlayerCharacterFromActorInfo()->HasAuthority())
		{
			// 대시 요청
			Server_GetLastInputAndUpdateRotationForDash(GetValidNormalizeInputVector());		
		}
	}
}

void UEosPlayerGameplayAbility::ApplyDash()
{
	// 워프 & 프리딕션키 형성
	const FName WarpKey = FName(TEXT("DashDirection"));
	FPredictionKey PredKey = GetCurrentActivationInfo().GetActivationPredictionKey();

	// 핸들 생성 (스태미나 이펙트)
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CostControlEffectClass, GetAbilityLevel());

	// 태그 코스트 연결
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle,
		EosGameplayTags::Player_SetByCaller_Status_Stamina,
		DashCost
	);

	// ASC 저장
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();

	if (ASC && SpecHandle.IsValid())
	{
		// 이펙트 발동 및 핸들 보관
		FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf
		(
			*SpecHandle.Data.Get(),
			PredKey
		);
		// Sequence 2. 방향 조절
		FRotator RotFromX = UKismetMathLibrary::MakeRotFromX(GetValidNormalizeInputVector());
		GetPlayerCharacterFromActorInfo()->Server_AddOrUpdateWarpTargetfromLocationandRotation(WarpKey, FVector::ZeroVector, RotFromX);

		// Sequence 3. 거리 조절 (트레이스)
		if (GetPlayerCharacterFromActorInfo()->HasAuthority())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::MakeRandomColor(), FString(TEXT("DashSuccess")));
			FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
			float CurrentDashData = UEosFunctionLibrary::GetScalableFloatValueAtLevel(DashData, GetAbilityLevel());

			FVector DashDistanceFromActorLoc = (CachedLastDirection * CurrentDashData) + PlayerLocation;
			FVector UpVectorToDashDist = GetPlayerCharacterFromActorInfo()->GetActorUpVector() * -1.f * TraceDistance;
			//UE_LOG(LogTemp, Warning, TEXT("UpVectorToDashDist = %s"), *UpVectorToDashDist.ToString());

			FVector EndPoint = UpVectorToDashDist + DashDistanceFromActorLoc;

			UWorld* World = GetWorld();

			// 대시거리 판별 라인 트레이스
			if (World)
			{
				FHitResult HitResult;
				bool bHit = World->LineTraceSingleByObjectType
				(
					HitResult,
					DashDistanceFromActorLoc,
					EndPoint,
					ECC_WorldStatic
				);
				if (bHit)
				{
					// 히트 위치로 워프
					GetPlayerCharacterFromActorInfo()->Server_AddOrUpdateWarpTargetfromLocation(HitResult.ImpactPoint);
				}
				else
				{
					// 히트가 없으면 워프 비활성화
					const FName DashDistance = FName(TEXT("DashDistance"));
					GetPlayerCharacterFromActorInfo()->Server_RemoveWarpTarget(DashDistance);
				}
			}
		}
	}
}

FVector UEosPlayerGameplayAbility::GetValidNormalizeInputVector()
{
	// 마지막 입력 벡터 저장
	FVector LastInputVector = GetPlayerCharacterFromActorInfo()->GetLastMovementInputVector();
	
	// 정규화
	FVector LastInputNorm = LastInputVector.GetSafeNormal(0.0001);
	
	// 입력 여부 검증
	if (FVector::ZeroVector != LastInputNorm)
	{
		// 정규화된 방향 벡터 반환
		return LastInputNorm;
	}
	else
	{
		// 비입력 시 전방 벡터 반환
		return GetCurrentForwardVector();
	}
}

FVector UEosPlayerGameplayAbility::GetCurrentForwardVector()
{
	// 전방 벡터 반환
	return GetPlayerCharacterFromActorInfo()->GetActorForwardVector();
}

void UEosPlayerGameplayAbility::SelectWeaponAttachment(USkeletalMeshComponent* InSkeletalMesh, AEosBaseWeapon* InFoundWeapon)
{
	if(InFoundWeapon)
	{
		// 어태치 옵션 설정
		FAttachmentTransformRules AttachmentRules
		(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepWorld,
			false
		);

		// 무기 타입 가져와서 Type별 어태치
		switch(InFoundWeapon->WeaponEquipType)
		{
		// 왼손
		case EWeaponEquipType::LeftHand : 
			// 파티클 전용 무기 대응
			if(InFoundWeapon->bIsParticleWeapon)
			{
				const FName& Socket = FName(TEXT("Weapon_L"));
				GetPlayerCharacterFromActorInfo()->Multicast_SpawnWeaponParticleL(InSkeletalMesh, InFoundWeapon, Socket);
			}
			InFoundWeapon->AttachToComponent
			(
				InSkeletalMesh,
				AttachmentRules,
				"Weapon_L"
			);
			break;

		// 오른손
		case EWeaponEquipType::RightHand :
			// 파티클 전용 무기 대응
			if (InFoundWeapon->bIsParticleWeapon)
			{
				const FName& Socket = FName(TEXT("Weapon_R"));
				GetPlayerCharacterFromActorInfo()->Multicast_SpawnWeaponParticleR(InSkeletalMesh, InFoundWeapon, Socket);
			}
			InFoundWeapon->AttachToComponent
			(
				InSkeletalMesh,
				AttachmentRules,
				"Weapon_R"
			);
			break;

		// 양손
		case EWeaponEquipType::TwoHands:
			// 파티클 전용 무기 대응
			if (InFoundWeapon->bIsParticleWeapon)
			{
				const FName& SocketL = FName(TEXT("Weapon_L"));
				GetPlayerCharacterFromActorInfo()->Multicast_SpawnWeaponParticleL(InSkeletalMesh, InFoundWeapon, SocketL);
				const FName& SocketR = FName(TEXT("Weapon_R"));
				GetPlayerCharacterFromActorInfo()->Multicast_SpawnWeaponParticleSubR(InSkeletalMesh, InFoundWeapon, SocketR);
			}
			InFoundWeapon->AttachToComponent
			(
				InSkeletalMesh,
				AttachmentRules,
				"Weapon_L"
			);
			InFoundWeapon->SubWeaponMesh->AttachToComponent
			(
				InSkeletalMesh,
				AttachmentRules,
				"Weapon_R"
			);
			break;
		}
	}
}
