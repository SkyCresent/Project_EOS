// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EosBaseCombatComponent.h"
#include "Net/UnrealNetwork.h"
#include "Items/Weapon/EosBaseWeapon.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

void UEosBaseCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEosBaseCombatComponent, CurrentEquippedWeaponTag);
}

void UEosBaseCombatComponent::Server_ToggleWeaponCollision_Implementation(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
    // 무기 콜리전 토글
    ToggleWeaponCollision(bShouldEnable, ToggleDamageType);
}

void UEosBaseCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
    // 토글할 대미지 타입 점검
    if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
    {
        // 현재 무기 저장
        AEosBaseWeapon* CurrentWeapon = GetCharacterCurrentEquippedWeapon();

        check(CurrentWeapon);

        // bool 값을 노출하여 ON/OFF 조정
        if (bShouldEnable)
        {
            CurrentWeapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString(TEXT("On QueryOnly")));
        }
        else
        {
            CurrentWeapon->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            OverlappedActors.Empty();
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString(TEXT("No Collision")));
        }
    }

    // 다른 타입일 경우(반복)
    // 만일 ToggleDamageType에 무기 위치 타입이 추가될 경우 else if로 전환 바람.
    else
    {
        AEosBaseWeapon* CurrentWeapon = GetCharacterCurrentEquippedWeapon();

        check(CurrentWeapon);

        if (bShouldEnable)
        {
            CurrentWeapon->GetSubWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        }
        else
        {
            CurrentWeapon->GetSubWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            OverlappedActors.Empty();
        }
    }
}

void UEosBaseCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTag, AEosBaseWeapon* InWeaponRegister, bool bIsRegisterAsEquippedWeapon)
{
    // 맵에 태그 및 무기를 삽입
    PlayerCarriedWeaponMap.Emplace(InWeaponTag, InWeaponRegister);

    // 무기 오버랩 시에 사용할 델리게이트에 바인딩
    InWeaponRegister->OnWepaonHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
    InWeaponRegister->OnWeaponEndHitFromTarget.BindUObject(this, &ThisClass::OnEndHitFromTargetActor);

    // 무기 장착 상태면...
    if (bIsRegisterAsEquippedWeapon)
    {
        // 현재 장착 무기 태그에 현재 태그 대입
        CurrentEquippedWeaponTag = InWeaponTag;
    }
}

void UEosBaseCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UEosBaseCombatComponent::OnEndHitFromTargetActor(AActor* HitActor)
{
}

AEosBaseWeapon* UEosBaseCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
    // 무기 맵 컨테이너에 해당 태그 키가 있는 지 확인
    if (PlayerCarriedWeaponMap.Contains(InWeaponTagToGet))
    {
        // 있으면 찾아와서 무기를 반환
        if (AEosBaseWeapon* const* FoundWeapon = PlayerCarriedWeaponMap.Find(InWeaponTagToGet))
        {
            return *FoundWeapon;
        }
    }
    return nullptr;
}

AEosBaseWeapon* UEosBaseCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
    
    if(!CurrentEquippedWeaponTag.IsValid()) return nullptr;

    // 현재 장착중인 무기 태그를 이용해서 무기 반환
    return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}