// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_SpawnEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnEnemiesDelegate, const TArray<AEosBaseEnemyCharacter*>&, SpawnedEnemies);

class AEosBaseEnemyCharacter;

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UAbilityTask_SpawnEnemy : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Eos | AbilityTask", meta=(DisplayName = "Wait Gameplay Event And Spawn Enemies", Hide = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", NumToSpawn = "1", RandomSpawnRadius = "200"))
	static UAbilityTask_SpawnEnemy* WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag, TSoftClassPtr<AEosBaseEnemyCharacter> SoftEnemyClassToSpawn, int32 NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius);

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate OnSpawnFinished;

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate DidNotSpawn;

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
private:
	FGameplayTag CachedEventTag;
	TSoftClassPtr<AEosBaseEnemyCharacter> CachedSoftEnemyClassToSpawn;
	int32 CachedNumToSpawn;
	FVector CachedSpawnOrigin;
	float CachedRandomSpawnRadius;
	FDelegateHandle DelegateHandle;

	void OnGameplayEventReceived(const FGameplayEventData* InPlayload);
	void OnEnemyClassLoaded();
};
