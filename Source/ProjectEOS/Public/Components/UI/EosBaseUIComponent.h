// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/EosPawnBaseActorComponent.h"
#include "EosBaseUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewCurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChanged, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float, NewStamina);

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosBaseUIComponent : public UEosPawnBaseActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnManaChanged OnManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStaminaChanged OnStaminaChanged;
	
};
