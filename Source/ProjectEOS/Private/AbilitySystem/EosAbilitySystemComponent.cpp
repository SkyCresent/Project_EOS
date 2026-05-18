// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/EosAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/EosPlayerGameplayAbility.h"
#include "EosGameplayTags.h"

void UEosAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}
	// 발동 가능 어빌리티 loop
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// 입력된 Tag와 Abilities 일치여부 검증
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;
		
		// 토글 타입인지 추가 검증
		if (InInputTag.MatchesTag(EosGameplayTags::InputTag_Toggle))
		{
			// 발동되어 있으면 취소
			if (AbilitySpec.IsActive())
			{
				CancelAbilityHandle(AbilitySpec.Handle);
			}
			// 아니면 발동 시도
			else 
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
		// 발동 시도
		else
		{
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UEosAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	// 유효성 && 태그 일치 여부 검증(가드타입)
	if (InInputTag.IsValid() && InInputTag.MatchesTag(EosGameplayTags::InputTag_Guard))
	{
		// 발동 가능 어빌리티 loop
		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			// 입력된 Tag와 Abilities 일치여부 검증 && 발동 여부 검증
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
			{
				CancelAbilityHandle(AbilitySpec.Handle);
				return;
			}
		}
	}

	// 유효성 && 태그 일치 여부 검증(연속타입)
	if (InInputTag.IsValid() && InInputTag.MatchesTag(EosGameplayTags::InputTag_Continue))
	{
		// 발동 가능 어빌리티 loop
		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			// 입력된 Tag와 Abilities 일치여부 검증 && 발동 여부 검증
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
			{
				CancelAbilityHandle(AbilitySpec.Handle);
				return;
			}
		}
	}
	return;
}

void UEosAbilitySystemComponent::GrantPlayerWeaponAbilities(const TArray<FPlayerAbilitySet>& InWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutAbilitySpecHandles)
{
	if (InWeaponAbilities.IsEmpty()) return;
	
	// 능력 배열 loop
	for (const FPlayerAbilitySet& AbilitySet : InWeaponAbilities)
	{
		// 유효하지 않은 GA면 재루프
		if (!AbilitySet.IsValid()) continue;
		
		// 스펙 형성
		FGameplayAbilitySpec AbilitySpec(AbilitySet.GrantAbility);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		
		// 스펙 전달 및 능력 부여
		OutAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UEosAbilitySystemComponent::RemoveGrantedPlayerWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	
	if (InSpecHandlesToRemove.IsEmpty()) return;

	// 능력 배열 loop
	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
		{
			// 어빌리티 제거
			ClearAbility(SpecHandle);
		}
	}
	// 무기 핸들 비우기
	InSpecHandlesToRemove.Empty();
}

bool UEosAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());
	
	// 빈 GA 스펙 배열
	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;

	// 실행 가능하고 Tag와 일치하는 모든 어빌리티 스펙을 배열에 삽입
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);
	
	// 해당 배열 비었으면...
	if (!FoundAbilitySpecs.IsEmpty())
	{
		// 배열 크기 -1만큼(Index match) 랜덤 수 대입
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		// 배열에 랜덤 수 입력하여 발동시킬 어빌리티 선정
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(SpecToActivate);

			// 미발동이면 발동 시도
			if (!SpecToActivate->IsActive())
			{
				return TryActivateAbility(SpecToActivate->Handle);
			}
	}
	return false;
}
