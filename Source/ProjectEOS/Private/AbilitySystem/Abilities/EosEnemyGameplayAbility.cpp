// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/EosEnemyGameplayAbility.h"
#include "AbilitySystem/Abilities/EosGameplayAbility.h"
#include "Character/EnemyCharacter/EosBaseEnemyCharacter.h"
#include "Components/Combat/EosEnemyCombatComponent.h"
#include "AbilitySystem/EosAbilitySystemComponent.h"
#include "EosGameplayTags.h"

AEosBaseEnemyCharacter* UEosEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedBaseEnemyCharacter.IsValid())
	{
		CachedBaseEnemyCharacter = Cast<AEosBaseEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedBaseEnemyCharacter.IsValid() ? CachedBaseEnemyCharacter.Get() : nullptr;
}

UEosEnemyCombatComponent* UEosEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}

FGameplayEffectSpecHandle UEosEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
	check(EffectClass);
	// 컨텍스트 핸들 생성
	FGameplayEffectContextHandle ContextHandle = GetEosAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	// 스펙 핸들 생성
	FGameplayEffectSpecHandle EffectSpecHandle = GetEosAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec
	(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	// 매개변수로 받은 대미지를 SetByCaller로 전달
	EffectSpecHandle.Data->SetSetByCallerMagnitude
	(
		EosGameplayTags::Common_SetByCaller_BaseDamage,
		InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
	);
	return EffectSpecHandle;
}

FGameplayEffectSpecHandle UEosEnemyGameplayAbility::MakeEnemyUnderAttackEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, AActor* InInstigator, float InLevel)
{
	check(EffectClass);
	// 컨텍스트 핸들 생성
	FGameplayEffectContextHandle ContextHandle = GetEosAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(InInstigator, InInstigator);

	// 스펙 핸들 생성
	FGameplayEffectSpecHandle EffectSpecHandle = GetEosAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec
	(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	return EffectSpecHandle;
}