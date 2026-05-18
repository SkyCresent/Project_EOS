// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/EosGameplayAbility.h"
#include "GameplayEffectTypes.h"
#include "EosPlayerGameplayAbility.generated.h"

class AEosBasePlayerCharacter;
class AEosBasePlayerController;
class AEosBaseWeapon;

/**
 *
 */
UCLASS()
class PROJECTEOS_API UEosPlayerGameplayAbility : public UEosGameplayAbility
{
	GENERATED_BODY()

/// NET FUNCTION
protected:

	UFUNCTION(Server, Reliable)
	void Server_GetLastInputAndUpdateRotationForDash(FVector InCachedLastDirection);

	UFUNCTION(Server, Reliable)
	void Server_GetLastInputAndUpdateRotationForAttack(FVector InCachedLastDirection);

	UFUNCTION(Client,Reliable)
	void Client_ApplyDash();

/// ENGINE FUNCTION
public:	
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
	
protected:

private:

/// CUSTOM FUNCTION
public:
	UFUNCTION(BlueprintPure, Category = "EOS | SpecHandle")
	FGameplayEffectSpecHandle MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);
	
	UFUNCTION(BlueprintPure, Category = "EOS | SpecHandle")
	FGameplayEffectSpecHandle MakeSkillDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InSkillDamage, FGameplayTag InCurrentAttackTypeTag);

	UFUNCTION(BlueprintCallable, Category = "EOS | Dash")
	void ConsumeDirectionInput();

	UFUNCTION(BlueprintCallable, Category = "EOS | Dash")
	void ComputeDashDirectionAndDistance();

	UFUNCTION()
	void ApplyDash();

protected:

	FVector GetCurrentForwardVector();
	FVector GetValidNormalizeInputVector();

	UFUNCTION(BlueprintCallable, Category = "EOS | WeaponAttachment")
	void SelectWeaponAttachment(USkeletalMeshComponent* InSkeletalMesh, AEosBaseWeapon* InFoundWeapon);

private:

/// CUSTOM VARIABLE

public:
	UPROPERTY(BlueprintReadOnly, Replicated, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AEosBasePlayerCharacter> CachedBasePlayerCharacter;
	
	UPROPERTY(BlueprintReadOnly, Replicated, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AEosBasePlayerController> CachedBasePlayerController;

protected:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "EOS | Player | Direction")
	FVector CachedLastDirection{FVector::ZeroVector};
	
	UPROPERTY(EditDefaultsOnly, Category = "EOS | Spec")
	TSubclassOf<UGameplayEffect> CostControlEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EOS | DashCost")
	float DashCost{ -15 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EOS | DashCost")
	float TraceDistance{ 400 };

	UPROPERTY(EditDefaultsOnly, Category = "EOS | ScalableFloat")
	FScalableFloat DashData;

private:

///GETTER FUNCTION
public:
	UFUNCTION(BlueprintPure, Category = "EOS | Ability")
	AEosBasePlayerCharacter* GetPlayerCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "EOS | Ability")
	AEosBasePlayerController* GetPlayerControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "EOS | Ability")
	UEosPlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

};
