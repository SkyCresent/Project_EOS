// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/EosGameplayAbility.h"
#include "AbilitySystem/EosAbilitySystemComponent.h"
#include "Components/Combat/EosBaseCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Skills/EosBaseProjectile.h"

void UEosGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    // 어빌리티가 OnGiven이면 즉시 발동
    if (AbilityActivationPolicy == EEosAbilityActivationPolicy::OnGiven)
    {
        ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
    }
}

void UEosGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    
    if (AbilityActivationPolicy == EEosAbilityActivationPolicy::OnGiven)
    {
        if (ActorInfo)
        {
            ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
        }
    }
}

int32 UEosGameplayAbility::GetGameplayEffectStackCountByTag(UEosAbilitySystemComponent* ASC, TSubclassOf<UGameplayEffect> EffectClass)
{
    if(!ASC || !EffectClass) return 0;
    
    // 발동 중인 모든 이펙트 핸들을 수집
    FGameplayEffectQuery Query;
    const TArray<FActiveGameplayEffectHandle>& Handles = ASC->GetActiveEffects(Query);

    for (const FActiveGameplayEffectHandle& Handle : Handles)
    {
        // 핸들 배열로부터 개별 핸들의 이펙트를 비교 검사
        const FActiveGameplayEffect* ActiveGE = ASC->GetActiveGameplayEffect(Handle);
        if (ActiveGE && ActiveGE->Spec.Def && ActiveGE->Spec.Def->GetClass() == EffectClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("Matched GE: %s | StackCount: %d"), *ActiveGE->Spec.Def->GetName(), ActiveGE->Spec.GetStackCount());
            // 조건에 일치하는 이펙트의 스펙에서 스택 수를 반환
            return ActiveGE->Spec.GetStackCount();
        }
    }
    // 일치하는 핸들 없음
    UE_LOG(LogTemp, Warning, TEXT("GameplayEffect %s not active."), *EffectClass->GetName());
    return 0;
}

UEosBaseCombatComponent* UEosGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
    return GetAvatarActorFromActorInfo()->FindComponentByClass<UEosBaseCombatComponent>();
}

UEosAbilitySystemComponent* UEosGameplayAbility::GetEosAbilitySystemComponentFromActorInfo() const
{
    return Cast<UEosAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UEosGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
    // 적용 시킬 대상 액터의 ASC 수집
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    check(TargetASC && InSpecHandle.IsValid());

    // 타겟에 이펙트 적용
    return GetEosAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget
    (
        *InSpecHandle.Data,
        TargetASC
    );
}

FActiveGameplayEffectHandle UEosGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EEosSuccessType& OutSuccessType)
{
    // 이펙트 핸들 저장
    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

    // 핸들로 이펙트 적용여부 평가하여 핀으로 노출
    OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EEosSuccessType::Successful : EEosSuccessType::Failed;
    
    // 이펙트 핸들 반환
    return ActiveGameplayEffectHandle;
}

void UEosGameplayAbility::ApplyEffectSpecHandle(const FGameplayEffectSpecHandle& EffectSpecHandle)
{
    UE_LOG(LogTemp, Warning, TEXT("Effect applied to ability: %s"), *GetNameSafe(this));
}

AActor* UEosGameplayAbility::SpawnActorWithEffectSpec(TSubclassOf<AActor> ActorClass, FTransform SpawnTransform, const FGameplayEffectSpecHandle& EffectSpecHandle, AActor* Instigator)
{
    if (!GetWorld() || !ActorClass || !GetAvatarActorFromActorInfo()->HasAuthority())return nullptr;

    FActorSpawnParameters SpawnParms;
    SpawnParms.Owner = Instigator;
    SpawnParms.Instigator = Instigator ? Instigator->GetInstigator() : nullptr;

    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnParms);

    if (SpawnedActor)
    {
        AEosBaseProjectile* Projectile = Cast<AEosBaseProjectile>(SpawnedActor);
        if (Projectile)
        {
            Projectile->SetDamageEffectSpecHandle(EffectSpecHandle);
        }
    }
    return SpawnedActor;
}