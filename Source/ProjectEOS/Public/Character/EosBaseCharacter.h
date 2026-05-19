// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/EosCombatInterface.h"
#include "EosBaseCharacter.generated.h"

class UEosAbilitySystemComponent;
class UEosAttributeSet;
class UEosDataAsset_BaseStartUpData;
class UMotionWarpingComponent;
class AEosBaseBossIntroManager;

UCLASS()
class PROJECTEOS_API AEosBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IEosCombatInterface
{
	GENERATED_BODY()


/// ENGINE FUNCTION

public:
	// Sets default values for this character's properties
	AEosBaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/// CUSTOM FUNCTION

public:
	virtual UEosBaseCombatComponent* GetPawnCombatComponent() const override;
	
protected:
	UFUNCTION()
	virtual void OnCutSceneFinished();

/// VARIABLE


/// REFERENCE VARIABLE
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem")
	TSoftObjectPtr<UEosDataAsset_BaseStartUpData> StartUpData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionWarpingComponent")
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Sequence")
	AEosBaseBossIntroManager* BossIntroManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sequence")
	TSubclassOf<AActor> BossIntroManagerClass;

/// REFERENCE GETTER FUNCTION
public:
	FORCEINLINE virtual UEosAbilitySystemComponent* GetEosAbilitySystemComponent() const
	{
		return nullptr;
	}

	FORCEINLINE UEosAttributeSet* GetEosAttributeSet() const
	{
		return nullptr;
	}
};