// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/InitializeData/EosDataAsset_BaseStartUpData.h"
#include "EosDataAsset_PlayerStartUpData.generated.h"

class UEosGameplayAbility;
class UGameplayEffect;
struct FPlayerAbilitySet;

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosDataAsset_PlayerStartUpData : public UEosDataAsset_BaseStartUpData
{
	GENERATED_BODY()

/// ENGINE FUNCTION

	
/// CUSTOM FUNCTION

public:
	virtual void GiveAbilitySystemComponent(UEosAbilitySystemComponent* ASC, int32 ApplyLevel = 1) override;

protected:

private:
	

/// CUSTOM VARIABLE

public:
	

protected:
	


private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData" /*TODO Check TitleProperty*/)
	TArray<FPlayerAbilitySet> PlayerStartUpAbilitySets;

};
