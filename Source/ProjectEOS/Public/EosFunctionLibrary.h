// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "Enums/EosEnumTypes.h"
#include "EosFunctionLibrary.generated.h"

class UEosAbilitySystemComponent;
class UEosBaseCombatComponent;

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

///CUSTOM FUNCTION ONLY
public:

	static UEosAbilitySystemComponent* GetNativeEosAbilitySystemComponent(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Eos | FunctionLibrary")
	static void RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	UFUNCTION(BlueprintCallable, Category = "Eos | FunctionLibrary")
	static void RemoveReplicatedGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	UFUNCTION(BlueprintCallable, Category = "Eos | FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Eos | FunctionLibrary")
	static void AddReplicatedGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Eos | FunctionLibrary", meta = (DisplayName = "Does Actor have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EEosConfirmType& OutConfirmType);

	static UEosBaseCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);
	
	UFUNCTION(BlueprintCallable, Category = "Eos | FunctionLibrary", meta = (DisplayName = "GetBaseCombatComponentFromActor", ExpandEnumAsExecs = "OutValidType"))
	static UEosBaseCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EEosValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "Eos | FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFlaot, float InLevel = 1.f);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Eos | NetMode", meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject"))
	static bool IsListenServer(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Eos | FunctionLibrary")
	static bool IsValidGuard(AActor* InAttacker, AActor* InDefender);

	UFUNCTION(BlueprintCallable, Category = "Eos | FunctionLibrary")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	UFUNCTION(BlueprintPure, Category = "Eos | FunctionLibrary")
	static FGameplayTag ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InReacter, float& OutAngleDiffrence);

	UFUNCTION(BlueprintPure, Category = "Eos | FunctionLibrary")
	static bool CheckTargetIsEnemy(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "Eos | FunctionLibrary")
	static bool CheckTextLength(const FString InString, int32 MinLength, int32 MaxLength);

};
