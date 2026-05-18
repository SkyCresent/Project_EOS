// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EosPlayerCombatComponent.h"
#include "Items/Weapon/EosBasePlayerWeapon.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EosGameplayTags.h"
#include "Struct/EosStructTypes.h"
#include "EosFunctionLibrary.h"
#include "AbilitySystem/EosAbilitySystemComponent.h"

AEosBasePlayerWeapon* UEosPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<AEosBasePlayerWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

AEosBasePlayerWeapon* UEosPlayerCombatComponent::GetPlayerCurrentEquippedWeapon() const
{
    return Cast<AEosBasePlayerWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UEosPlayerCombatComponent::GetPlayerCurrentWeaponDamageAtLevel(float InLevel) const
{
    return GetPlayerCurrentEquippedWeapon()->PlayerWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

// 사용 여부 판단 후 삭제 ~
float UEosPlayerCombatComponent::GetPlayerCurrentSkillDamageAtLevel(ESkillButtons InSkillButtons, float InLevel) const
{
    float OutValue = 0;
    switch (SkillButtons)
    {
    case ESkillButtons::Q:
        OutValue = GetPlayerCurrentEquippedWeapon()->PlayerWeaponData.SkillBaseDamage.GetValueAtLevel(InLevel);
        break;

    case ESkillButtons::E:
        OutValue = GetPlayerCurrentEquippedWeapon()->PlayerWeaponData.SkillBaseDamage.GetValueAtLevel(InLevel);
        break;

    case ESkillButtons::R:
        OutValue = GetPlayerCurrentEquippedWeapon()->PlayerWeaponData.SkillBaseDamage.GetValueAtLevel(InLevel);
        break;

    case ESkillButtons::F:
        OutValue = GetPlayerCurrentEquippedWeapon()->PlayerWeaponData.SkillBaseDamage.GetValueAtLevel(InLevel);
        break;
    }
    return OutValue;
}
// ~ 사용 여부 판단 후 삭제 

void UEosPlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
    APawn* HitPawn = Cast<APawn>(HitActor);
    // 타격된 액터가 배열에 있으면 종료
    if (OverlappedActors.Contains(HitActor))
    {
        return;
    }
    // 타격된 액터를 배열에 보관
    OverlappedActors.AddUnique(HitActor);

    // 이벤트 데이터 생성
    FGameplayEventData Data;
    Data.Instigator = GetOwningPawn();
    Data.Target = HitActor;
    
    // 적 여부 확인
    if(UEosFunctionLibrary::CheckTargetIsEnemy(GetOwner()->GetInstigator(), HitPawn))
    {
        // 카운터 가능 여부 확인
        if (UEosFunctionLibrary::NativeDoesActorHaveTag(GetOwner(), EosGameplayTags::Player_Status_Counter) && UEosFunctionLibrary::NativeDoesActorHaveTag(HitActor, EosGameplayTags::Enemy_Status_Counterable))
        {
            // 유효 가드(카운터) 방향 확인
            if(UEosFunctionLibrary::IsValidGuard(GetOwner(), HitActor))
            {
                // 카운터 성공 발신
                UAbilitySystemBlueprintLibrary::SendGameplayEventToActor
                (
                    HitActor,
                    EosGameplayTags::Enemy_Event_SuccessCounter,
                    Data
                );
                UEosAbilitySystemComponent* ASC = UEosFunctionLibrary::GetNativeEosAbilitySystemComponent(GetOwner());

                // 유저 피드백 효과(큐)
                if (ASC)
                {
                    FGameplayCueParameters Params;
                    FVector GreatSwordLocation = FVector(GetCharacterCurrentEquippedWeapon()->GetActorLocation());

                    Params.Instigator = GetOwner();
                    Params.SourceObject = GetOwner();
                    Params.Location = GreatSwordLocation;
                    Params.RawMagnitude = 0.4f;
                    
                    ASC->ExecuteGameplayCue(EosGameplayTags::GameplayCue_Feedback_HoldAnim, Params);
                }
                //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Sending Success Counter Event!!"));
                
                // 대미지 발신
                UAbilitySystemBlueprintLibrary::SendGameplayEventToActor
                (
                    GetOwningPawn(),
                    EosGameplayTags::Common_Event_MeleeHit,
                    Data
                );
            }
        }
        else
        {
            // 대미지 발신
            UAbilitySystemBlueprintLibrary::SendGameplayEventToActor
            (
                GetOwningPawn(),
                EosGameplayTags::Common_Event_MeleeHit,
                Data
            );
        }
    }
}

void UEosPlayerCombatComponent::OnEndHitFromTargetActor(AActor* HitActor)
{
}