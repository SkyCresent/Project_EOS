// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Character/EosBaseCharacter.h"
#include "EosBaseEnemyCharacter.generated.h"

class UEosEnemyCombatComponent;
class UEosEnemyUIComponent;
class UBoxComponent;
class UEosAbilitySystemComponent;
class UEosAttributeSet;

/**
 * 
 */
UCLASS()
class PROJECTEOS_API AEosBaseEnemyCharacter : public AEosBaseCharacter
{
	GENERATED_BODY()
	AEosBaseEnemyCharacter();

/// ENGINE FUNCTION
protected:
	virtual void PossessedBy(AController* NewController) override;

/// CUSTOM FUNCTION
public:
	virtual UEosBaseCombatComponent* GetPawnCombatComponent() const override;

protected:
	virtual void OnCutSceneFinished() override;
/// CUSTOM VARIABLE

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEosEnemyCombatComponent* EnemyCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEosEnemyUIComponent* EnemyUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* EnemyBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UEosAbilitySystemComponent* EosAbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem")
	UEosAttributeSet* EosAttributeSet;

protected:

private:
	void InitEnemyStartUpData();

/// GETTER FUNCTION
public:
	FORCEINLINE virtual UEosAbilitySystemComponent* GetEosAbilitySystemComponent() const
	{
		return EosAbilitySystemComponent;
	}
	FORCEINLINE UEosAttributeSet* GetEosAttributeSet() const
	{
		return EosAttributeSet;
	}
	FORCEINLINE UEosEnemyCombatComponent* GetEnemyCombatComponent() const
	{
		return EnemyCombatComponent;
	}
};