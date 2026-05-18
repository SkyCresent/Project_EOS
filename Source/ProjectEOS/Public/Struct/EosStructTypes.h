// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "EosStructTypes.generated.h"

class UEosGameplayAbility;
class UInputMappingContext;
class UEosPlayerLinkedAnimLayer;
class UEosPrimaryDataAsset_ItemData;

/**
 * 
 */

//PROJECTEOS_API

USTRUCT(BlueprintType)
struct  FPlayerAbilitySet
{

	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UEosGameplayAbility> GrantAbility;

/// CUSTOM FUNCTION
	bool IsValid() const;


	// 기본 생성자
	FPlayerAbilitySet()
		: InputTag(FGameplayTag()) // FGameplayTag는 기본 생성자로 초기화
		, GrantAbility(nullptr)    // TSubclassOf는 nullptr로 초기화
	{}
};

USTRUCT(BlueprintType)
struct  FPlayerWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UEosPlayerLinkedAnimLayer> WeaponAnimLayerToLink;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FPlayerAbilitySet> DefaultWeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat SkillBaseDamage;

	// 기본 생성자
	FPlayerWeaponData()
		: WeaponAnimLayerToLink(nullptr)               // TSubclassOf<UEosPlayerLinkedAnimLayer>는 nullptr로 초기화
		, WeaponInputMappingContext(nullptr)           // UInputMappingContext*는 nullptr로 초기화
		, DefaultWeaponAbilities()                     // TArray<FPlayerAbilitySet>은 빈 배열로 초기화
		, WeaponBaseDamage(FScalableFloat(0.0f))       // FScalableFloat은 0.0f로 초기화
		, SkillBaseDamage(FScalableFloat(0.0f))        // FScalableFloat은 0.0f로 초기화
	{}
};

USTRUCT(BlueprintType)
struct  FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGuid Guid;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UEosPrimaryDataAsset_ItemData> DataAssets;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Quantity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 EnhancementLevel;

	// 기본 생성자
	FInventorySlot()
		: Guid(0, 0, 0, 0)
		, DataAssets(nullptr)
		, Quantity(0)
		, EnhancementLevel(0)
	{}
};