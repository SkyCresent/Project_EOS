// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/InitializeData/EosDataAsset_BaseStartUpData.h"
#include "EosDataAsset_EnemyStartUpData.generated.h"

class UEosEnemyGameplayAbility;

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosDataAsset_EnemyStartUpData : public UEosDataAsset_BaseStartUpData
{
	GENERATED_BODY()
///CUSTOM FUNCTION
public:
	virtual void GiveAbilitySystemComponent(UEosAbilitySystemComponent* ASC, int32 ApplyLevel = 1);

///CUSTOM VARIABLE
private:
	UPROPERTY(EditAnywhere, Category = "StartUpData")
	TArray<TSubclassOf<UEosEnemyGameplayAbility>>EnemyCombatAbilities;
};
