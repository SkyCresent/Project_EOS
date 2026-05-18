// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/EosPlayerGA_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/PlayerCharacter/EosBasePlayerCharacter.h"
#include "Widget/EosBaseWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "PlayerController/EosBasePlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "EosFunctionLibrary.h"
#include "EosGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemBlueprintLibrary.h"

void UEosPlayerGA_TargetLock::Server_RequestRotationControl_Implementation (AEosBasePlayerCharacter* InRotateCharacter, FRotator TargetRot)
{
	// 로테이션 조정
	InRotateCharacter->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	// 클라이언트에 재요청
	Client_RequestRotationControl(InRotateCharacter, TargetRot);
}

void UEosPlayerGA_TargetLock::Client_RequestRotationControl_Implementation(AEosBasePlayerCharacter* InRotateCharacter, FRotator TargetRot)
{
	// 로테이션 조정
	InRotateCharacter->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
}

void UEosPlayerGA_TargetLock::Server_RequestInitTargetLockMovement_Implementation(UCharacterMovementComponent* InMovementComp)
{
	// 원래 이동속도 보존
	CachedDefaultMaxWalkSpeed = InMovementComp->MaxWalkSpeed;
	if(IsLocallyControlled())
	{
		if(GetAvatarActorFromActorInfo()->HasAuthority())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Walk : %f"), CachedDefaultMaxWalkSpeed));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Walk : %f"), CachedDefaultMaxWalkSpeed));
		}
	}
	// 로코모션 활성화하고 이동속도 감소
	InMovementComp->bOrientRotationToMovement = false;
	InMovementComp->MaxWalkSpeed = TargetLockMaxWalkSpeed;
	// 클라에 같은 세팅 요청
	Client_RequestInitTargetLockMovement(InMovementComp);
}

void UEosPlayerGA_TargetLock::Client_RequestInitTargetLockMovement_Implementation(UCharacterMovementComponent* InMovementComp)
{
	// 로코모션 활성화하고 이동속도 감소

	InMovementComp->bOrientRotationToMovement = false;
	InMovementComp->MaxWalkSpeed = TargetLockMaxWalkSpeed;
}

void UEosPlayerGA_TargetLock::Server_RequestResetTargetLockMovement_Implementation(UCharacterMovementComponent* InResetCharacter)
{
	// 이동속도 원상복구 후 로코모션 비활성화

	InResetCharacter->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
	InResetCharacter->bOrientRotationToMovement = true;
}

void UEosPlayerGA_TargetLock::Server_RequestResetDefaultWalkSpeed_Implementation()
{
	CachedDefaultMaxWalkSpeed = 0.f;
}

void UEosPlayerGA_TargetLock::Server_RequestCheckEnemyStatus_Implementation(AActor* CurrentTarget)
{
	// 서버에서 태그 체크
	if(!CurrentTarget || UEosFunctionLibrary::NativeDoesActorHaveTag(CurrentTarget, EosGameplayTags::Common_Status_Dead))
	{
		// 죽었으면 클라이언트로 전달
		Client_RequestCheckEnemyStatus();
		return;
	}
}

void UEosPlayerGA_TargetLock::Client_RequestCheckEnemyStatus_Implementation()
{
	// 어빌리티 취소
	CancelTargetLockAbility();
}

void UEosPlayerGA_TargetLock::Server_RequestCheckEnemyStatusForTryLockOnTarget_Implementation(AActor* CurrentTarget)
{
	// 사망 여부 검증
	if (!CurrentTarget || UEosFunctionLibrary::NativeDoesActorHaveTag(CurrentTarget, EosGameplayTags::Common_Status_Dead))
	{
		// 클라에서 취소 처리
		Client_RequestCheckEnemyStatus();
		return;
	}
	else
	{
		// 클라에 타겟 락온 요청
		Client_RequestCheckEnemyStatusForTryLockOnTarget();
	}
}

void UEosPlayerGA_TargetLock::Client_RequestCheckEnemyStatusForTryLockOnTarget_Implementation()
{
	// 위젯 락온
	LockOnTarget();
	// 무브먼트 설정
	InitTargetLockMovement();
	// 컨텍스트 부여
	InitTargetLockContext();
}

void UEosPlayerGA_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 락온 시도
	TryLockOnTarget();
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEosPlayerGA_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 이동 관련 설정 원상복구
	ResetTargetLockMovement();
	// 컨텍스트 회수
	ResetTargetLockContext();
	// 지정된 변수 모두 초기화
	CleanUp();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEosPlayerGA_TargetLock::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEosPlayerGA_TargetLock, CachedDefaultMaxWalkSpeed);
}

void UEosPlayerGA_TargetLock::OnTargetLockTick(float DeltaTime)
{
	if(!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		// 서버를 통해 적 사망 여부 검증
		Server_RequestCheckEnemyStatus(CurrentLockedActor);
		// 위젯을 적 위치에 고정
		SetTargetLockWidgetPosition();
	}
	else
	{
		// 적 사망 여부 검증
		if(IsLocallyControlled())
		{
			if (!CurrentLockedActor || UEosFunctionLibrary::NativeDoesActorHaveTag(CurrentLockedActor, EosGameplayTags::Common_Status_Dead))
			{
				CancelTargetLockAbility();
				return;
			}
			// 위젯을 적 위치에 고정
			SetTargetLockWidgetPosition();
		}
	}
	// 플레이어 회피 여부 검증
	const bool bShouldOverrideRotation = !UEosFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), EosGameplayTags::Player_Status_Dash);

	if (bShouldOverrideRotation)
	{
		// 바라보는 회전 값 저장
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetPlayerCharacterFromActorInfo()->GetActorLocation(), CurrentLockedActor->GetActorLocation());
		
		// 카메라 오프셋 조정
		LookAtRot -= FRotator(CameraOffsetDistance, 0.f, 0.f);

		// 컨트롤러 회전과 로테이션 보간 값
		const FRotator CurrentControlRot = PC->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot, LookAtRot, DeltaTime, TargetLookRotationInterpSpeed);
		
		// 호스트 검증
		if(GetAvatarActorFromActorInfo()->HasAuthority())
		{
			if(IsLocallyControlled())
			{
				// 에너미 방향으로 로테이션 강제 조정
				PC->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
				GetPlayerCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
			}
		}
		// 클라이언트
		else
		{
			AEosBasePlayerCharacter* RequestCharacter = GetPlayerCharacterFromActorInfo();
			// 서버에 회전 보정 요청
			Server_RequestRotationControl(RequestCharacter, TargetRot);
			PC->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		}
	}
}

void UEosPlayerGA_TargetLock::SwitchTarget(const FGameplayTag& InSwitchDirectionTag)
{
	// 트레이스로 타겟을 배열에 담음
	GetAvailableActorsToLock();

	// 빈배열 형성
	TArray<AActor* >ActorsOnLeft;
	TArray<AActor* >ActorsOnRight;
	AActor* NewTargetToLock = nullptr;

	// 새로운 타겟을 담을 좌/우 배열 전달
	GetAvailableActorsAroundTarget(ActorsOnLeft, ActorsOnRight);

	// 왼쪽으로 요청이 왔으면 가장 가까운 왼쪽 타겟을 저장
	if (InSwitchDirectionTag == EosGameplayTags::Player_Event_SwitchTarget_Left)
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnLeft);
	}
	// 오른쪽
	else
	{
		NewTargetToLock = GetNearestTargetFromAvailableActors(ActorsOnRight);
	}
	// 새로운 타겟이 유효하면 타겟 교체
	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}
}

void UEosPlayerGA_TargetLock::TryLockOnTarget()
{
	// 락온 가능한 액터 탐색
	GetAvailableActorsToLock();

	// 탐색한 액터 배열이 비었으면 어빌리티 취소
	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}
	// 가장 가까운 유효 액터 반환
	CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);

	Server_RequestCheckEnemyStatusForTryLockOnTarget(CurrentLockedActor);
}

void UEosPlayerGA_TargetLock::LockOnTarget()
{
	// 타겟락온 위젯 생성
	DrawTargetLockWidget();
	// 타겟락온 위젯 위치 선정
	SetTargetLockWidgetPosition();
}

void UEosPlayerGA_TargetLock::GetAvailableActorsToLock()
{
	// 배열 비우기
	AvailableActorsToLock.Empty();
	TArray<FHitResult> BoxTraceHits;

	// 멀티 박스 트레이스
	UKismetSystemLibrary::BoxTraceMultiForObjects
	(
		GetPlayerCharacterFromActorInfo(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation() * GetPlayerCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		GetPlayerCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits,
		true
	);

	// 히트된 액터들 배열에 삽입
	for (const FHitResult& TraceHit : BoxTraceHits)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			AvailableActorsToLock.AddUnique(HitActor);
		}
	}
}

AActor* UEosPlayerGA_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
	// 플레이어와 가까운 액터 선정
	float ClosestDistance = 0.f;
	return UGameplayStatics::FindNearestActor(GetPlayerCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
}

void UEosPlayerGA_TargetLock::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsLeft, TArray<AActor*>& OutActorsRight)
{
	// 현재 타겟 없거나 배열이 비어있으면 어빌리티 취소
	if (!CurrentLockedActor || AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}

	// 플레이어 위치 저장
	const FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	// 상호 간의 정규화된 방향 벡터 저장
	const FVector PlayerToCurrentLockedActorNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();
	
	// 배열 loop
	for (AActor* FoundTarget : AvailableActorsToLock)
	{
		// 타겟이 없거나 같으면 루프 지속
		if (!FoundTarget || FoundTarget == CurrentLockedActor) continue;

		// 새로운 타겟과의 방향 벡터 저장
		const FVector PlayerToFoundTargetNormalized = (FoundTarget->GetActorLocation() - PlayerLocation).GetSafeNormal();
		// 교차곱 시행
		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentLockedActorNormalized, PlayerToFoundTargetNormalized);
		
		// 결과에 따라서 좌/우 분류
		if (CrossResult.Z > 0.f)
		{
			OutActorsRight.AddUnique(FoundTarget);
		}
		else
		{
			OutActorsLeft.AddUnique(FoundTarget);
		}
	}
}

void UEosPlayerGA_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UEosPlayerGA_TargetLock::CleanUp()
{
	AvailableActorsToLock.Empty();

	CurrentLockedActor = nullptr;

	if (TargetLockWidget)
	{
		TargetLockWidget->RemoveFromParent();
	}

	TargetLockWidget = nullptr;

	TargetWidgetLockSize = FVector2D::ZeroVector;

	Server_RequestResetDefaultWalkSpeed();
	CachedDefaultMaxWalkSpeed = 0.f;
}

void UEosPlayerGA_TargetLock::ResetTargetLockMovement()
{
	UCharacterMovementComponent* RequestMovementComp = GetPlayerCharacterFromActorInfo()->GetCharacterMovement();
	if (CachedDefaultMaxWalkSpeed > 0.f)
	{
		// 서버에 원상복구 요청
		Server_RequestResetTargetLockMovement(RequestMovementComp);
		// 이동속도 복원 및 로코모션 비활성화
		RequestMovementComp->MaxWalkSpeed = CachedDefaultMaxWalkSpeed;
		RequestMovementComp->bOrientRotationToMovement = true;
	}
}

void UEosPlayerGA_TargetLock::ResetTargetLockContext()
{
	// 컨트롤러 불일치 시 반환
	if (!GetAvatarActorFromActorInfo()->GetInstigatorController()) return;
	
	// 호스트 검증
	else if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		if (IsLocallyControlled())
		{
			// 컨텍스트 제거에 필요한 매개변수 수집
			PC = Cast<AEosBasePlayerController>(GetAvatarActorFromActorInfo()->GetInstigatorController());
			const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

			check(Subsystem);

			// 매핑 컨텍스트 제거
			Subsystem->RemoveMappingContext(TargetLockMappingContext);
		}
	}

	// 클라이언트 검증
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		// 컨텍스트 제거에 필요한 매개변수 수집
		PC = Cast<AEosBasePlayerController>(GetAvatarActorFromActorInfo()->GetInstigatorController());
		const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		check(Subsystem);

		// 매핑 컨텍스트 제거
		Subsystem->RemoveMappingContext(TargetLockMappingContext);
	}
}

void UEosPlayerGA_TargetLock::SetTargetLockWidgetPosition()
{
	// 위젯 혹은 감지된 액터가 없으면 어빌리티 취소
	if (!TargetLockWidget || !CurrentLockedActor)
	{
		CancelTargetLockAbility();
		return;
	}
	FVector2D ScreenPosition;
	
	// 에너미 위치를 UI 좌표계로 전환
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition
	(
		PC,												// 뷰포트 소유 컨트롤러
		CurrentLockedActor->GetActorLocation(),			// 월드에서 변환할 지점
		ScreenPosition,									// 좌표 출력받을 변수
		true											// 플레이어 뷰포트 별 상대 좌표 여부
	);

	// 아직 아무 값도 안담겨 있으면
	if (TargetWidgetLockSize == FVector2d::ZeroVector)
	{
		// 위젯 트리 순회
		TargetLockWidget->WidgetTree->ForEachWidget
		(
			[this](UWidget* FoundWidget)
			{
				// 사이즈 박스 찾으면
				if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
				{
					// 찾은 사이즈 박스의 높이, 넓이를 대입
					TargetWidgetLockSize.X = FoundSizeBox->GetWidthOverride();
					TargetWidgetLockSize.Y = FoundSizeBox->GetHeightOverride();
				}
			}
		);
	}
	// 중앙으로 보정하고 액터 좌표로 설정
	ScreenPosition -= (TargetWidgetLockSize / 2.f);
	TargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UEosPlayerGA_TargetLock::DrawTargetLockWidget()
{
	// 자신의 컨트롤러 아니면 반환
	if (!GetActorInfo().IsLocallyControlled()) return;

	PC = Cast<AEosBasePlayerController>(GetAvatarActorFromActorInfo()->GetInstigatorController());
	if (!PC)
	{
		// 컨트롤러 캐스트 실패 시 재시도
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UEosPlayerGA_TargetLock::DrawTargetLockWidget);
		return;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), PC));
	}

	if (!TargetLockWidget)
	{
		// 생성된 위젯이 없으면 생성하고 출력
		checkf(TargetLockWidgetClass, TEXT("Forgot to assign TargetLockWidgetClass, Check GA_TargetLock"));
		TargetLockWidget = CreateWidget<UEosBaseWidget>(PC, TargetLockWidgetClass);
		check(TargetLockWidget);
		TargetLockWidget->AddToViewport();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Done Add viewport")));
	}
}

void UEosPlayerGA_TargetLock::InitTargetLockMovement()
{
	UCharacterMovementComponent* RequestMovement = GetPlayerCharacterFromActorInfo()->GetCharacterMovement();
	// 서버에 이동 관련 초기화 요청
	Server_RequestInitTargetLockMovement(RequestMovement);
}

void UEosPlayerGA_TargetLock::InitTargetLockContext()
{
	// 호스트 검증
	if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		if (IsLocallyControlled())
		{
			PC = Cast<AEosBasePlayerController>(GetAvatarActorFromActorInfo()->GetInstigatorController());
			const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();

			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

			check(Subsystem);

			// 매핑 컨텍스트 부여
			Subsystem->AddMappingContext(TargetLockMappingContext, 3);
		}
	}
	// 클라이언트
	if(!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		PC = Cast<AEosBasePlayerController>(GetAvatarActorFromActorInfo()->GetInstigatorController());
		const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		check(Subsystem);

		// 매핑 컨텍스트 부여
		Subsystem->AddMappingContext(TargetLockMappingContext, 3);
	}
}