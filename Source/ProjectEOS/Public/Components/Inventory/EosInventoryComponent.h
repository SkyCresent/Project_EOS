// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/EosPawnBaseActorComponent.h"
#include "EosInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosInventoryComponent : public UEosPawnBaseActorComponent
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

};