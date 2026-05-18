// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/EosAbilitySystemComponent.h"
#include "EosAttributeSet.generated.h"

class UEosBaseUIComponent;
class AEosBasePlayerState;
class AEosBasePlayerCharacter;

// 편의성 함수 등록 매크로 (Get&Set&Init)
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UEosAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UEosAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UEosAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData CurrentStamina;
	ATTRIBUTE_ACCESSORS(UEosAttributeSet, CurrentStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UEosAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData CurrentMana;
	ATTRIBUTE_ACCESSORS(UEosAttributeSet, CurrentMana)

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UEosAttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UEosAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UEosAttributeSet, DefensePower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UEosAttributeSet, DamageTaken)

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Burserker Attributes")
	bool bUseHPWhenStaminaExhausted = false;

	UEosBaseUIComponent* UIComponent;
	AEosBasePlayerState* PS;
	AEosBasePlayerCharacter* Char;

	//UPROPERTY(EditDefaultsOnly, Category = "GameplayEffects | Death")
	//TSubclassOf<UGameplayEffect> GE_Death;

protected:
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

};
