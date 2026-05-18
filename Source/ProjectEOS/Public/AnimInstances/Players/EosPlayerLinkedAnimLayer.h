// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/EosCharacterAnimInstance.h"
#include "EosPlayerLinkedAnimLayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosPlayerLinkedAnimLayer : public UEosBaseAnimInstance
{
	GENERATED_BODY()

/// CUSTOM FUNCTION
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UEosCharacterAnimInstance* GetCharacterAnimInstance() const;
};