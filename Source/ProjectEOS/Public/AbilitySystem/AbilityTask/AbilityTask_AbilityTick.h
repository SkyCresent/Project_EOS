// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_AbilityTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UAbilityTask_AbilityTick : public UAbilityTask
{
	GENERATED_BODY()
/// ENGINE FUNCTION
public:
	UAbilityTask_AbilityTick();

	UFUNCTION(BlueprintCallable, Category = "Eos | AbilityTask", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_AbilityTick* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);

	virtual void TickTask(float DeltaTime) override; 

	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegate OnAbilityTaskTick;
};
