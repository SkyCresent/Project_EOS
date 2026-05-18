// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon/EosBasePlayerWeapon.h"
#include "Net/UnrealNetwork.h"

void AEosBasePlayerWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEosBasePlayerWeapon, PlayerWeaponData);
}

void AEosBasePlayerWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AEosBasePlayerWeapon::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}
