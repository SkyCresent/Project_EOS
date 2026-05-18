// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter/EosBaseEnemyCharacter.h"
#include "Components/Combat/EosEnemyCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DataAssets/InitializeData/EosDataAsset_EnemyStartUpData.h"
#include "Engine/AssetManager.h"
#include "Components/UI/EosEnemyUIComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "AbilitySystem/EosAbilitySystemComponent.h"
#include "AbilitySystem/EosAttributeSet.h"

AEosBaseEnemyCharacter::AEosBaseEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
	
	EnemyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyCollision"));
	EnemyBoxComponent->SetupAttachment(GetRootComponent());
	EnemyBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EnemyBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	
	EnemyCombatComponent = CreateDefaultSubobject<UEosEnemyCombatComponent>(TEXT("EnemyCombatComponent"));
	EnemyUIComponent = CreateDefaultSubobject<UEosEnemyUIComponent>(TEXT("EnemyUIComponent"));

	EosAbilitySystemComponent = CreateDefaultSubobject<UEosAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	EosAbilitySystemComponent->SetIsReplicated(true);
	EosAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	EosAttributeSet = CreateDefaultSubobject<UEosAttributeSet>(TEXT("AbilityAttributeSet"));
}

UEosBaseCombatComponent* AEosBaseEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

void AEosBaseEnemyCharacter::OnCutSceneFinished()
{
	// 애니메이션 강제 중단
	GetMesh()->PlayAnimation(nullptr, false);
}

void AEosBaseEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// 캐릭터 초기화
	if (EosAbilitySystemComponent)
	{
		// ActorInfo 초기화
		EosAbilitySystemComponent->InitAbilityActorInfo(this, this);
		ensureMsgf(!StartUpData.IsNull(), TEXT("Forgot Assign Data Assets to %s"), *GetName());
		InitEnemyStartUpData();
	}
}

void AEosBaseEnemyCharacter::InitEnemyStartUpData()
{
	if (StartUpData.IsNull()) return;

	// 에셋 비동기 로드 요청
	UAssetManager::GetStreamableManager().RequestAsyncLoad
	(
		// 에셋 지정
		StartUpData.ToSoftObjectPath(),
		// 로드 완료되면 스타트업 데이터 로드 및 실행
		FStreamableDelegate::CreateLambda
		(
			[this]()
			{
				if (UEosDataAsset_BaseStartUpData* LoadedData = StartUpData.Get())
				{
					LoadedData->GiveAbilitySystemComponent(EosAbilitySystemComponent);
				}
			}
		)
	);
}