// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EosPrimaryDataAsset_ItemData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTEOS_API UEosPrimaryDataAsset_ItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "ItemData")
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, Category = "ItemData")
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, Category = "ItemData")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	bool IsStackableItem = false;

	UPROPERTY(EditDefaultsOnly, Category = "ItemData", meta = (EditCondition = "IsStackableItem"))
	int32 StackSize = 1;

	UPROPERTY(EditDefaultsOnly, Category = "ItemData")
	int32 BuyPrice;

	UPROPERTY(EditDefaultsOnly, Category = "ItemData")
	int32 SellPrice;
};