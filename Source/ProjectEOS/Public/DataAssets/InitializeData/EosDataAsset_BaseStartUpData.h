// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EosDataAsset_BaseStartUpData.generated.h"

class UEosAbilitySystemComponent;
class UGameplayEffect;
class UEosGameplayAbility;

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosDataAsset_BaseStartUpData : public UDataAsset
{
	GENERATED_BODY()

/// ENGINE FUNCTION

/// CUSTOM FUNCTION

public:
	virtual void GiveAbilitySystemComponent(UEosAbilitySystemComponent* ASC, int32 ApplyLevel = 1);

protected:
	void GrantAbilities(const TArray<TSubclassOf<UEosGameplayAbility>>& InGiveAbilities, UEosAbilitySystemComponent* ASC, int32 ApplyLevel = 1);

private:

/// CUSTOM VARIABLE

public:

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UEosGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UEosGameplayAbility>> ReactAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

private:
	
};