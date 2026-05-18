// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/EosBaseCombatComponent.h"
#include "EosEnemyCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosEnemyCombatComponent : public UEosBaseCombatComponent
{
	GENERATED_BODY()
	
	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnEndHitFromTargetActor(AActor* HitActor) override;

protected: 

	void Client_SendReactEvent();

};