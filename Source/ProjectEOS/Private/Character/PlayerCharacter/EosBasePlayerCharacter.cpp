// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter/EosBasePlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAssets/InitializeData/EosDataAsset_BaseStartUpData.h"
#include "AbilitySystem/EosAbilitySystemComponent.h"
#include "Components/Combat/EosPlayerCombatComponent.h"
#include "EosGameplayTags.h"
#include "Components/Inventory/EosInventoryComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Components/UI/EosPlayerUIComponent.h"
#include "AbilitySystem/EosAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Weapon/EosBaseWeapon.h"
#include "MotionWarpingComponent.h"
#include "PlayerController/EosBasePlayerController.h"
#include "Managers/EosBaseBossIntroManager.h"
#include "Net/UnrealNetwork.h"
#include "HUD/EosVillageHUD.h"
#include "Character/NonePlayerCharacter/EosBaseNonePlayerCharacter.h"


AEosBasePlayerCharacter::AEosBasePlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 0.f);
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	PlayerCombatComponent = CreateDefaultSubobject<UEosPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	PlayerUIComponent = CreateDefaultSubobject<UEosPlayerUIComponent>(TEXT("PlayerUIComponent"));
}

void AEosBasePlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEosBasePlayerCharacter, SkillType);
}

void AEosBasePlayerCharacter::BindIntroDelegate()
{
	if(BossIntroManager)
	{
		// 컷씬 종료 바인딩
		BossIntroManager->OnFinishedScene.AddUniqueDynamic
		(
			this,
			&AEosBasePlayerCharacter::OnCutSceneFinished
		);
	}
}

void AEosBasePlayerCharacter::EndIntroBossScene_Implementation()
{
}

void AEosBasePlayerCharacter::Server_AddOrUpdateWarpTargetfromLocationandRotation_Implementation(const FName InName, FVector InTargetLocation, FRotator InTargetRotation)
{
	Multicast_AddOrUpdateWarpTargetfromLocationandRotation(InName, InTargetLocation, InTargetRotation);
}

void AEosBasePlayerCharacter::Multicast_AddOrUpdateWarpTargetfromLocationandRotation_Implementation(const FName InName, FVector InTargetLocation, FRotator InTargetRotation)
{
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(InName, InTargetLocation, InTargetRotation);
}

void AEosBasePlayerCharacter::Server_AddOrUpdateWarpTargetfromLocation_Implementation(const FVector InTargetLocation)
{
	Multicast_AddOrUpdateWarpTargetfromLocation(InTargetLocation);
}

void AEosBasePlayerCharacter::Multicast_AddOrUpdateWarpTargetfromLocation_Implementation(const FVector InTargetLocation)
{
	const FName DashDistance = FName(TEXT("DashDistance"));
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(DashDistance, InTargetLocation);
}

void AEosBasePlayerCharacter::Server_RemoveWarpTarget_Implementation(const FName InWarpTarget)
{
	Multicast_RemoveWarpTarget(InWarpTarget);
}

void AEosBasePlayerCharacter::Multicast_RemoveWarpTarget_Implementation(const FName InWarpTarget)
{
	MotionWarpingComponent->RemoveWarpTarget(InWarpTarget);
}


void AEosBasePlayerCharacter::Multicast_SpawnWeaponParticleL_Implementation(USkeletalMeshComponent* InSkeletalMesh, AEosBaseWeapon* InFoundWeapon, const FName& InWeaponSocket)
{
	// 어태치 옵션
	FAttachmentTransformRules AttachmentRules
	(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepWorld,
		false
	);
	// 파티클 어태치
	UGameplayStatics::SpawnEmitterAttached
	(
		InFoundWeapon->ParticleWeaponL,
		InSkeletalMesh,
		InWeaponSocket,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget
	);
}

void AEosBasePlayerCharacter::Multicast_SpawnWeaponParticleR_Implementation(USkeletalMeshComponent* InSkeletalMesh, AEosBaseWeapon* InFoundWeapon, const FName& InWeaponSocket)
{
	FAttachmentTransformRules AttachmentRules
	(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepWorld,
		false
	);

	UGameplayStatics::SpawnEmitterAttached
	(
		InFoundWeapon->ParticleWeaponR,
		InSkeletalMesh,
		InWeaponSocket,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget
	);
}

void AEosBasePlayerCharacter::Multicast_SpawnWeaponParticleSubR_Implementation(USkeletalMeshComponent* InSkeletalMesh, AEosBaseWeapon* InFoundWeapon, const FName& InWeaponSocket)
{
	FAttachmentTransformRules AttachmentRules
	(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepRelative,
		EAttachmentRule::KeepWorld,
		false
	);

	UGameplayStatics::SpawnEmitterAttached
	(
		InFoundWeapon->ParticleWeaponR,
		InSkeletalMesh,
		InWeaponSocket,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget
	);
}


void AEosBasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	checkf(PlayerClassType != EPlayerClassType::Empty, TEXT("Your 'EPlayerClassType' has EMPTY! Check your Character."));
}

void AEosBasePlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (GetEosAbilitySystemComponent())
	{
		// ActorInfo 초기화
		GetEosAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState<AEosBasePlayerState>(), this);
		ensureMsgf(!StartUpData.IsNull(), TEXT("Forgot Assign Data Assets to %s"), *GetName());
	}
	if (!StartUpData.IsNull())
	{
		// 스타트업 데이터 동기 로드
		if (UEosDataAsset_BaseStartUpData* LoadedData = StartUpData.LoadSynchronous())
		{
			LoadedData->GiveAbilitySystemComponent(GetEosAbilitySystemComponent());
		}
	}

	EosPlayerController = Cast<AEosBasePlayerController>(GetInstigatorController());
	// 레이드에서 인트로 델리게이트 연결
	if (EosPlayerController->bIsRaid)
	{
		GetWorld()->GetTimerManager().SetTimer(EndIntroSequenceHandle, this, &ThisClass::BindIntroDelegate, 0.1f);
	}
}

UEosBaseCombatComponent* AEosBasePlayerCharacter::GetPawnCombatComponent() const
{
	return PlayerCombatComponent;
}

void AEosBasePlayerCharacter::OnCutSceneFinished()
{
	EndIntroBossScene();
}


AEosBasePlayerController* AEosBasePlayerCharacter::GetEosPlayerController()
{
	if (!EosPlayerController)
	{
		EosPlayerController = Cast<AEosBasePlayerController>(GetController());
		return EosPlayerController;
	}
	else return EosPlayerController;
}

UEosInventoryComponent* AEosBasePlayerCharacter::BP_GetInventoryComponent() const
{
	return GetInventoryComponent();
}

EPlayerSkillType AEosBasePlayerCharacter::GetPlayerSkillType() const
{
	return SkillType;
}

void AEosBasePlayerCharacter::SetPlayerSkillType(EPlayerSkillType NewSkillType)
{
	SkillType = NewSkillType;
}
