// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/EosBaseCombatComponent.h"
#include "EosPlayerCombatComponent.generated.h"

UENUM(BlueprintType)
enum class ESkillButtons : uint8
{
	Q,
	E,
	R,
	F
};
/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosPlayerCombatComponent : public UEosBaseCombatComponent
{
	GENERATED_BODY()

///SERVER FUNCTION
protected:

///CUSTOM FUNCTION
public:

	UFUNCTION(BlueprintCallable, Category = "Player | Combat")
	AEosBasePlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	UFUNCTION(BlueprintCallable, Category = "Player | Combat")
	AEosBasePlayerWeapon* GetPlayerCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category = "Player | Combat")
	float GetPlayerCurrentWeaponDamageAtLevel(float InLevel) const;
	UFUNCTION(BlueprintCallable, Category = "Player | Combat")
	float GetPlayerCurrentSkillDamageAtLevel(ESkillButtons InSkillButtons, float InLevel) const;
	
	virtual void OnHitTargetActor(AActor* HitActor) override;
	virtual void OnEndHitFromTargetActor(AActor* HitActor) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Skills")
	ESkillButtons SkillButtons;
};
