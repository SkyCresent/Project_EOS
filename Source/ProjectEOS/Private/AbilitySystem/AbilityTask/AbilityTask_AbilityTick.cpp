// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/AbilityTask_AbilityTick.h"

UAbilityTask_AbilityTick::UAbilityTask_AbilityTick()
{
	// 틱 수신 여부
	bTickingTask = true;
}

UAbilityTask_AbilityTick* UAbilityTask_AbilityTick::ExecuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	// 태스크 인스턴스 생성
	UAbilityTask_AbilityTick* Node = NewAbilityTask<UAbilityTask_AbilityTick>(OwningAbility);

	return Node;
}

void UAbilityTask_AbilityTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	// 어빌리티 & 태스크 유효성 검증
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// 유효하면 델리게이트를 통해 델타타임 전달
		OnAbilityTaskTick.Broadcast(DeltaTime);
	}
	else
	{
		EndTask();
	}
}