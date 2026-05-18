// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Struct/EosStructTypes.h"
#include "EosAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()


/// CUSTOM FUNCTION
public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category = "EOS | Ability", meta = (ApplyLevel = "1"))
	void GrantPlayerWeaponAbilities(const TArray<FPlayerAbilitySet>& InWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "EOS | Ability")
	void RemoveGrantedPlayerWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "EOS | Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};