// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapon/EosBaseWeapon.h"
#include "Struct/EosStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "EosBasePlayerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEOS_API AEosBasePlayerWeapon : public AEosBaseWeapon
{
/// ENGINE FUNCTION
	GENERATED_BODY()
public:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

/// CUSTOM FUNCTION
public:
	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);
protected:

private:

/// CUSTOM VARIABLE
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "WeaponData")
	FPlayerWeaponData PlayerWeaponData;
	
protected:

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;

/// GETTER FUNCTION
public:
	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;
};