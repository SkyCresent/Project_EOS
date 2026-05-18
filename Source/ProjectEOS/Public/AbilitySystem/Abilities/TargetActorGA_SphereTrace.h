// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/EosCommonGA_TargetActor.h"
#include "TargetActorGA_SphereTrace.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTEOS_API ATargetActorGA_SphereTrace : public AEosCommonGA_TargetActor
{
	GENERATED_BODY()
protected:
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual bool LineTraceFN(FHitResult& OutHit) override;
	virtual void ConfirmTargetingAndContinue() override;
};
