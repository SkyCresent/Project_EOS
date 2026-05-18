// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/EosPawnBaseActorComponent.h"
#include "GameplayTagContainer.h"
#include "EosBaseCombatComponent.generated.h"

class AEosBaseWeapon;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	CurrentSubWeapon
};

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosBaseCombatComponent : public UEosPawnBaseActorComponent
{
	GENERATED_BODY()

/// ENGINE FUNCTION
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

protected:

private:

/// NET FUNCTION
public:
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Eos | Combat")
	void Server_ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	UFUNCTION(BlueprintCallable, Category = "Eos | Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

/// CUSTOM FUNCTION

public:
	UFUNCTION(BlueprintCallable, Category = "Player | Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTag, AEosBaseWeapon* InWeaponRegister, bool bIsRegisterAsEquippedWeapon = false);

	virtual void OnHitTargetActor(AActor* HitActor);
	virtual void OnEndHitFromTargetActor(AActor* HitActor);


protected:


private:

/// CUSTOM VARIABLE

public:
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Player | Combat")
	FGameplayTag CurrentEquippedWeaponTag;
	
protected:
	TArray<AActor*> OverlappedActors;

private:
	TMap<FGameplayTag, AEosBaseWeapon*> PlayerCarriedWeaponMap;

///GETTER FUNCTION
public:
	UFUNCTION(BlueprintCallable, Category = "Player | Combat")
	AEosBaseWeapon* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UFUNCTION(BlueprintCallable, Category = "Player | Combat")
	AEosBaseWeapon* GetCharacterCurrentEquippedWeapon() const;
	
};
