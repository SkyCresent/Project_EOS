// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/EosGameplayAbility.h"
#include "EosEnemyGameplayAbility.generated.h"

class AEosBaseEnemyCharacter;

UENUM(BlueprintType)
enum class EAttackScale : uint8
{
	LightAttack,
	HeavyAttack
};

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosEnemyGameplayAbility : public UEosGameplayAbility
{
	GENERATED_BODY()

///CUSTOM FUNCTION
public:

	UFUNCTION(BlueprintPure, Category = "EOS | Ability")
	AEosBaseEnemyCharacter* GetEnemyCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "EOS | Ability")
	UEosEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "EOS | Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect>EffectClass, const FScalableFloat& InDamageScalableFloat);

	UFUNCTION(BlueprintPure, Category = "EOS | Ability")
	FGameplayEffectSpecHandle MakeEnemyUnderAttackEffectSpecHandle(TSubclassOf<UGameplayEffect>EffectClass, AActor* InInstigator, float InLevel);


///CUSTOM VARIABLE

private:
	TWeakObjectPtr<AEosBaseEnemyCharacter> CachedBaseEnemyCharacter;
	TWeakObjectPtr<UEosEnemyCombatComponent> CachedEnemyCombatComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Scale", meta = (ToolTip = "Set the scale of the attack to determine the player's hit reaction level.", EditCondition = "bIsAttackGameAbility"))
	EAttackScale AttackScale = EAttackScale::LightAttack;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Attack Scale", meta = (ToolTip = "Please make sure to check only in attack ability."))
	bool bIsAttackGameAbility = false;

};
