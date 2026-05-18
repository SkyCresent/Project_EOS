// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Items/EosPrimaryDataAsset_EquipData.h"
#include "EosPrimaryDataAsset_WeaponData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTEOS_API UEosPrimaryDataAsset_WeaponData : public UEosPrimaryDataAsset_EquipData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	TSoftObjectPtr<UStaticMesh> LeftWeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	TSoftObjectPtr<UStaticMesh> RightWeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	TSoftObjectPtr<UParticleSystem> LeftParticleWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponData")
	TSoftObjectPtr<UParticleSystem> RightParticleWeapon;
};