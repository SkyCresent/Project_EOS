// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EosEnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EosGameplayTags.h"
#include "EosFunctionLibrary.h"

void UEosEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    APawn* HitPawn = Cast<APawn>(HitActor);
    APawn* OwnerPawn = Cast<APawn>(GetOwner()->GetInstigator());
    // 피격자가 이미 배열에 있다면 중단
    if (OverlappedActors.Contains(HitActor))
    {
        return;
    }
    
    // 배열에 없다면 추가
    OverlappedActors.AddUnique(HitActor);

    // 이벤트 데이터 생성
    FGameplayEventData Data;
    Data.Instigator = GetOwningPawn();
    Data.Target = HitActor;

    // 가드 & 철벽 등 플레이어 상태 조건 확인
	bool bIsValidGuard = false;
    bool bIsvalidRevenge = false;
	const bool bIsPlayerGuard = UEosFunctionLibrary::NativeDoesActorHaveTag(HitActor, EosGameplayTags::Player_Status_Guarding);
    const bool bIsPlayerIronwall = UEosFunctionLibrary::NativeDoesActorHaveTag(HitActor, EosGameplayTags::Player_Status_GreatSword_Ironwall);

    // 가드 중이면 가드 성공 이벤트 발송
	if (bIsPlayerGuard)
	{
		bIsValidGuard = UEosFunctionLibrary::IsValidGuard(GetOwner(), HitActor);
        if (bIsValidGuard)
        {
            UAbilitySystemBlueprintLibrary::SendGameplayEventToActor
            (
                HitActor,
                EosGameplayTags::Player_Event_GuardSuccess,
                Data
            );
            return;
        }
	}

    // 철벽 중이면 철벽 성공 이벤트 발송
    if (bIsPlayerIronwall)
    {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor
        (
            HitActor,
            EosGameplayTags::Player_Event_GreatSword_SuccessIronwall,
            Data
        );
    }
    
    // 가드가 아닐 경우 전부 대미지 처리
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor
    (
        GetOwningPawn(),
        EosGameplayTags::Common_Event_MeleeHit,
        Data
    );

}

void UEosEnemyCombatComponent::OnEndHitFromTargetActor(AActor* HitActor)
{
}