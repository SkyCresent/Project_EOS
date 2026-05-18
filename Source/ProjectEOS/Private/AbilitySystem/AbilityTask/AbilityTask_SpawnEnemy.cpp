// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/AbilityTask_SpawnEnemy.h"
#include "AbilitySystemComponent.h"
#include "Engine/AssetManager.h"
#include "NavigationSystem.h"
#include "Character/EnemyCharacter/EosBaseEnemyCharacter.h"

UAbilityTask_SpawnEnemy* UAbilityTask_SpawnEnemy::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag, TSoftClassPtr<AEosBaseEnemyCharacter> SoftEnemyClassToSpawn, int32 NumToSpawn, const FVector& SpawnOrigin, float RandomSpawnRadius)
{
	// 노드화
	UAbilityTask_SpawnEnemy* Node = NewAbilityTask<UAbilityTask_SpawnEnemy>(OwningAbility);
	Node->CachedEventTag = EventTag;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;

	return Node;
}

void UAbilityTask_SpawnEnemy::Activate()
{
	// 태그로 콜백 바인딩
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	DelegateHandle = Delegate.AddUObject(this, &ThisClass::OnGameplayEventReceived);
}

void UAbilityTask_SpawnEnemy::OnDestroy(bool bInOwnerFinished)
{
	// 태그로 바인드 해제
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);

	Delegate.Remove(DelegateHandle);
	
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_SpawnEnemy::OnGameplayEventReceived(const FGameplayEventData* InPlayload)
{
	if (ensure(!CachedSoftEnemyClassToSpawn.IsNull()))
	{
		// 소프트 오브젝트 비동기 로드
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad
		(
			CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
			// 로드 완료 델리게이트 바인딩
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded)
		);
	}
	else
	{
		// 클래스 비어있으면 소환 실패 방송
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AEosBaseEnemyCharacter*>());
		}
		// 태스크 종료
		EndTask();
		return;
	}
}

void UAbilityTask_SpawnEnemy::OnEnemyClassLoaded()
{
	UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
	UWorld* World = GetWorld();

	// 로드 실패 혹은 월드 컨텍스트 문제 시 실패 델리게이트 방송
	if (!LoadedClass || !World)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			DidNotSpawn.Broadcast(TArray<AEosBaseEnemyCharacter*>());
		}
		// 태스크 종료
		EndTask();
		return;
	}

	TArray<AEosBaseEnemyCharacter*> SpawnedEnemies;

	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int32 i = 0; i < CachedNumToSpawn; i++)
	{
		FVector RandomLocation;
		// 랜덤 포인트 산출
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(World, CachedSpawnOrigin, RandomLocation, CachedRandomSpawnRadius);
		
		// 약간의 공중에서 소환
		RandomLocation += FVector(0.f, 0.f, 150.f);
		// 시전자의 방향 값
		const FRotator SpawnFacingRotation = AbilitySystemComponent->GetAvatarActor()->GetActorForwardVector().ToOrientationRotator();

		AEosBaseEnemyCharacter* SpawnedEnemy = World->SpawnActor<AEosBaseEnemyCharacter>(LoadedClass, RandomLocation, SpawnFacingRotation);
		if (SpawnedEnemy)
		{
			SpawnedEnemies.Add(SpawnedEnemy);
		}
		// 태스크 유효성 검사
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			// 객체 스폰 되었으면
			if (!SpawnedEnemies.IsEmpty())
			{
				// 완료 방송
				OnSpawnFinished.Broadcast(SpawnedEnemies);
			}
			else
			{
				DidNotSpawn.Broadcast(TArray<AEosBaseEnemyCharacter*>());
			}

			EndTask();
		}
	}
}
