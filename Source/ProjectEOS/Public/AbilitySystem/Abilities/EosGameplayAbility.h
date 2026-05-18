// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Enums/EosEnumTypes.h"
#include "EosGameplayAbility.generated.h"

class UEosPlayerCombatComponent;
class UEosAbilitySystemComponent;

/**
 * 
 */
UENUM(BlueprintType)
enum class EEosAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

UCLASS()
class PROJECTEOS_API UEosGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

/// ENGINE FUNCTION

public:

protected:

	/** Called when the ability is given to an AbilitySystemComponent */
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

private:

/// CUSTOM FUNCTION

public:

protected:
	UFUNCTION(BlueprintPure, Category = "EOS | GameplayEffect")
	int32 GetGameplayEffectStackCountByTag(UEosAbilitySystemComponent* ASC, TSubclassOf<UGameplayEffect> EffectClass);

private:


/// CUSTOM VARIABLE

public:


protected:

	UPROPERTY(EditDefaultsOnly, Category = "AbilityActivationPolicy")
	EEosAbilityActivationPolicy AbilityActivationPolicy = EEosAbilityActivationPolicy::OnTriggered;

private:


/// GETTER FUNCTION
protected:
	UFUNCTION(BlueprintPure, Category = "EOS | Ability")
	UEosBaseCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "EOS | Ability")
	UEosAbilitySystemComponent* GetEosAbilitySystemComponentFromActorInfo() const;
	
	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "EOS | Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EEosSuccessType& OutSuccessType);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ApplyEffectSpecHandle(const FGameplayEffectSpecHandle& EffectSpecHandle);

	UFUNCTION(BlueprintCallable, Category = "EOS | Ability", meta = (DisplayName = "Spawn Actor with EffectSpecHandle"))
	AActor* SpawnActorWithEffectSpec(TSubclassOf<AActor>ActorClass, FTransform SpawnTransform, const FGameplayEffectSpecHandle& EffectSpecHandle, AActor* Instigator);
};
 