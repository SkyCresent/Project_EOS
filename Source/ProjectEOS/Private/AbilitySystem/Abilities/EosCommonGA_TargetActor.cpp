// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/EosCommonGA_TargetActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityWorldReticle.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "Skills/EosBaseProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Abilities/GameplayAbility.h"


AEosCommonGA_TargetActor::AEosCommonGA_TargetActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bUseLeftHandSocket = false;
	bUseTwoHandSocket = false;
	RightHandSocketName = FName("Weapon_R");
	LeftHandSocketName = FName("Weapon_L");
	TwoHandSocketName = FName("TwoHandSocket");
}

// 타겟팅 시작 시 호출 : Ability 및 컨트롤러 설정 + 리타클(Decal) 생성
void AEosCommonGA_TargetActor::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	OwningAbility = Ability;

	AController* OwnerController = Ability->GetOwningActorFromActorInfo()->GetInstigatorController();
	PrimaryPC = Cast<APlayerController>(OwnerController);
	MasterAIController = Cast<AAIController>(OwnerController);

	MyReticleActor = SpawnReticleActor(GetActorLocation(), GetActorRotation());
}

// 타겟팅 취소 시 호출 : 리타클(Decal) 제거
void AEosCommonGA_TargetActor::CancelTargeting()
{
	Super::CancelTargeting();
	DestroyReticleActors();
}

// 매 프레임 호출 : 트레이스를 통해 리타클(Decal) 위치 갱신
void AEosCommonGA_TargetActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FHitResult Hit;
	if (LineTraceFN(Hit))
	{
		if (MyReticleActor)
		{
			MyReticleActor->SetActorLocation(Hit.ImpactPoint, false, nullptr, ETeleportType::None);
		}
	}
	else
	{
		if (MyReticleActor)
		{
			MyReticleActor->SetActorLocation(Hit.TraceEnd, false, nullptr, ETeleportType::None);
		}
	}
}

// 게임 시작 시 호출 : 추가 초기화 가능
void AEosCommonGA_TargetActor::BeginPlay()
{
	Super::BeginPlay();
}

// 기본 라인트레이스 함수 (자식 클래스에서 오버라이드)
bool AEosCommonGA_TargetActor::LineTraceFN(FHitResult& OutHit)
{
	return false; // Default: No trace
}

// 타겟 확정 시 TargetData 생성 및 전송
void AEosCommonGA_TargetActor::ConfirmTargetingAndContinue()
{
	FHitResult Hit;
	FGameplayTag GameTag;
	if (LineTraceFN(Hit))
	{
		HandleEffectProjectile(Hit, GameTag);
		FGameplayAbilityTargetDataHandle TargetData;
		FGameplayAbilityTargetData_SingleTargetHit* HitData = new FGameplayAbilityTargetData_SingleTargetHit(Hit);
		TargetData.Add(HitData);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}

	DestroyReticleActors();
}

void AEosCommonGA_TargetActor::HandleEffectProjectile(const FHitResult& Hit, const FGameplayTag& EventTag)
{
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Player.Event.Spawn.Left")))
	{
		bUseLeftHandSocket = true;
		bUseTwoHandSocket = false;
	}
	else if (EventTag == FGameplayTag::RequestGameplayTag(FName("Player.Event.Spawn.TwoHands")))
	{
		bUseLeftHandSocket = false;
		bUseTwoHandSocket = true;
	}
	else
	{
		bUseLeftHandSocket = false;
		bUseTwoHandSocket = false;
	}
	if (!ProjectileClass || !HasAuthority()) return;

	AActor* EosActor = OwningAbility ? OwningAbility->GetOwningActorFromActorInfo() : nullptr;
	if (!EosActor) return;

	FTransform SpawnTransForm;

	FName SocketToUse;
	if (bUseTwoHandSocket)
	{
		SocketToUse = TwoHandSocketName;
	}
	else if (bUseLeftHandSocket)
	{
		SocketToUse = LeftHandSocketName;
	}
	else
	{
		SocketToUse = RightHandSocketName;
	}
	SocketToUse = bUseLeftHandSocket ? LeftHandSocketName : RightHandSocketName;

	if (USkeletalMeshComponent* Mesh = EosActor->FindComponentByClass<USkeletalMeshComponent>())
	{
		SpawnTransForm.SetLocation(Mesh->GetSocketLocation(ProjectileSpawnSocketName));
		SpawnTransForm.SetRotation(Mesh->GetSocketRotation(ProjectileSpawnSocketName).Quaternion());
	}
	else
	{
		SpawnTransForm.SetLocation(EosActor->GetActorLocation());
		SpawnTransForm.SetRotation(EosActor->GetActorRotation().Quaternion());
	}
	FActorSpawnParameters Params;
	Params.Owner = EosActor;
	Params.Instigator = EosActor->GetInstigator();

	AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransForm, Params);
	if (SpawnedProjectile)
	{
		SpawnedProjectile->SetReplicates(true);

		if (UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(SpawnedProjectile->GetRootComponent()))
		{
			RootComp->SetIsReplicated(true);
			RootComp->SetSimulatePhysics(false);
		}
		if (AEosBaseProjectile* Projectile = Cast<AEosBaseProjectile>(SpawnedProjectile))
		{
			Projectile->SetOwnerIgnoreCollision(EosActor);
			if (DamageEffectClass)
			{
				UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EosActor);
				if (ASC)
				{
					FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
					FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, 1.f, Context);
					if (EffectSpecHandle.IsValid())
					{
						Projectile->SetDamageEffectSpecHandle(EffectSpecHandle);
					}
				}
			}
		}
	}
}

// 리타클(Decal) 액터를 월드에 스폰
AGameplayAbilityWorldReticle* AEosCommonGA_TargetActor::SpawnReticleActor(FVector Location, FRotator Rotation)
{
	if (!CustomReticleClass || MyReticleActor) return MyReticleActor;

	AGameplayAbilityWorldReticle* Spawned = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(CustomReticleClass, Location, Rotation);

	if (Spawned)
	{
		Spawned->InitializeReticle(this, PrimaryPC, CustomReticleParams);
		Spawned->SetOwner(PrimaryPC);
		Spawned->SetReplicates(true);
	}

	return Spawned;
}
// 리타클(Decal) 액터 제거
void AEosCommonGA_TargetActor::DestroyReticleActors()
{
	if (MyReticleActor)
	{
		MyReticleActor->Destroy();
		MyReticleActor = nullptr;
	}
}
