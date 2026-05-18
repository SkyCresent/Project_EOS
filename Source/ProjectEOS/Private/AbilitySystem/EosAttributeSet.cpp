// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/EosAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "Components/UI/EosBaseUIComponent.h"
#include "Components/UI/EosPlayerUIComponent.h"
#include "EosFunctionLibrary.h"
#include "EosGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CoreSystem/PlayerState/EosBasePlayerState.h"
#include "Character/PlayerCharacter/EosBasePlayerCharacter.h"
#include "CoreSystem/GameMode/EosRaidGameMode.h"
#include "PlayerController/EosBasePlayerController.h"


// 값 초기화
UEosAttributeSet::UEosAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentStamina(1.f);
	InitMaxStamina(1.f);
	InitCurrentMana(1.f);
	InitMaxMana(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

// 이펙트 호출 시
void UEosAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// UIComponent
	PS = Cast<AEosBasePlayerState>(GetOwningActor());
	if (PS && PS->IsA<AEosBasePlayerState>())
	{
		Char = Cast<AEosBasePlayerCharacter>(PS->EosAbilitySystemComponent->GetAvatarActor());
		UIComponent = Cast<UEosBaseUIComponent>(Char->GetComponentByClass(UEosBaseUIComponent::StaticClass()));
	}
	else
	{
		UIComponent = Cast<UEosBaseUIComponent>(GetOwningActor()->GetComponentByClass(UEosBaseUIComponent::StaticClass()));
	}

	// UI는 이 구간에서 값을 도출하거나 가져가면 됩니다. ~
	// Applied Target이 체력 속성인 경우
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		// UI 컴포넌트에 변동사항 전달(현재 체력과 최대 체력)
		UIComponent->OnHealthChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());
	}

	// Applied Target이 스태미나 속성인 경우
	if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
	{
		const float NewCurrentStamina = FMath::Clamp(GetCurrentStamina(), 0.f, GetMaxStamina());

		SetCurrentStamina(NewCurrentStamina);

		// Stamina UI 연동
		if (Data.EvaluatedData.Magnitude != 0)
		{
			UIComponent->OnStaminaChanged.Broadcast(GetCurrentStamina() / GetMaxStamina());
		}
		// 버서커일 경우 스태미나 소진 시 체력 소진
		if (bUseHPWhenStaminaExhausted && NewCurrentStamina <= 0.f)
		{
			float HPToConsume = 10.f;
			float NewCurrentHealth = FMath::Clamp(GetCurrentHealth() - HPToConsume, 1.f, GetMaxHealth());
			SetCurrentHealth(NewCurrentHealth);

			UIComponent->OnHealthChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());
		}
	}

	// Applied Target이 마나 속성인 경우
	if (Data.EvaluatedData.Attribute == GetCurrentManaAttribute())
	{
		const float NewCurrentMana = FMath::Clamp(GetCurrentMana(), 0.f, GetMaxMana());

		SetCurrentMana(NewCurrentMana);
		
		// MP UI 연동
		if(Data.EvaluatedData.Magnitude != 0)
			UIComponent->OnManaChanged.Broadcast(GetCurrentMana() / GetMaxMana());
	}
	
	// Applied Target이 Damage Taken 속성일 경우
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float TakenDamage = GetDamageTaken();
		
		const float NewCurrentHealth = FMath::Clamp(OldHealth - TakenDamage, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);
		GEngine->AddOnScreenDebugMessage
		(
			-1, 10.f, FColor::Green, 
			FString::Printf(TEXT("OldHealth: %f, TakenDamage: %f, CurrentHealth: %f"),
			OldHealth,
			TakenDamage,
			NewCurrentHealth)
		);

		// HP 연동
		UIComponent->OnHealthChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());

		// Party HP 연동
		AEosRaidGameMode* GameMode = Cast<AEosRaidGameMode>(GetWorld()->GetAuthGameMode());
		if(GameMode && GameMode->GetPartyNum() > 1)
		{
			if (AEosBasePlayerController* PlayerController = Cast<AEosBasePlayerController>(GetOwningActor()->GetOwner()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Server");
				Cast<UEosPlayerUIComponent>(UIComponent)->OnServerUpdatePartyHP.Broadcast(PlayerController->GetGuid(), GetCurrentHealth() / GetMaxHealth());

				GameMode->UpdateParty(PlayerController->GetGuid(), GetCurrentHealth() / GetMaxHealth());
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "No Owner");
			}
		}
		
		if (GetCurrentHealth() <= 0.f)
		{
			//UEosAbilitySystemComponent* ASC = UEosFunctionLibrary::GetNativeEosAbilitySystemComponent(Data.Target.GetAvatarActor());
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s"), *ASC->GetFullName()));
			//FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GE_Death, 1.f, ASC->MakeEffectContext());

			//if (SpecHandle.IsValid())
			//{
			//	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("TagGranted")));
			//}
			UEosFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), EosGameplayTags::Common_Status_Dead);
		}

		if (UEosFunctionLibrary::NativeDoesActorHaveTag(Data.Target.GetAvatarActor(), EosGameplayTags::Enemy_Status_Boss))
		{
			FGameplayEventData EventData;
			if (GetCurrentHealth() / GetMaxHealth() <= 0.8f && !UEosFunctionLibrary::NativeDoesActorHaveTag(Data.Target.GetAvatarActor(), EosGameplayTags::Enemy_Status_Boss_Phase1))
			{
				/*UEosFunctionLibrary::RemoveGameplayFromActorIfFound(Data.Target.GetAvatarActor(), EosGameplayTags::Enemy_Status_Boss_PrePhase);*/
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.Target.GetAvatarActor(), EosGameplayTags::Enemy_Event_Boss_Phase1, EventData);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Now player entering Phase1")));
			}
			else if (GetCurrentHealth() / GetMaxHealth() <= 0.5f && !UEosFunctionLibrary::NativeDoesActorHaveTag(Data.Target.GetAvatarActor(), EosGameplayTags::Enemy_Status_Boss_Phase2))
			{
				/*UEosFunctionLibrary::RemoveGameplayFromActorIfFound(Data.Target.GetAvatarActor(), EosGameplayTags::Enemy_Status_Boss_Phase1);*/
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.Target.GetAvatarActor(), EosGameplayTags::Enemy_Event_Boss_Phase2, EventData);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Now player entering Phase2")));
			}
			else if (GetCurrentHealth() / GetMaxHealth() <= 0.3f && !UEosFunctionLibrary::NativeDoesActorHaveTag(Data.Target.GetAvatarActor(), EosGameplayTags::Enemy_Status_Boss_Phase3))
			{
				/*UEosFunctionLibrary::RemoveGameplayFromActorIfFound(Data.Target.GetAvatarActor(), EosGameplayTags::Enemy_Status_Boss_Phase2);*/
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.Target.GetAvatarActor(), EosGameplayTags::Enemy_Event_Boss_Phase3, EventData);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Now player entering Phase3")));
			}
		}
	}
	// ~ UI는 이 구간에서 값을 도출하거나 가져가면 됩니다.
}

// 클라이언트 UI 연동
void UEosAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEosAttributeSet, CurrentHealth, OldHealth);
	PS = Cast<AEosBasePlayerState>(GetOwningActor());
	if (PS && PS->IsA<AEosBasePlayerState>())
	{
		Char = Cast<AEosBasePlayerCharacter>(PS->EosAbilitySystemComponent->GetAvatarActor());
		if (!Char) return;
		UIComponent = Cast<UEosBaseUIComponent>(Char->GetComponentByClass(UEosBaseUIComponent::StaticClass()));
	}
	else
	{
		UIComponent = Cast<UEosBaseUIComponent>(GetOwningActor()->GetComponentByClass(UEosBaseUIComponent::StaticClass()));
	}
	if (!UIComponent) return;
	UIComponent->OnHealthChanged.Broadcast(GetCurrentHealth(), GetMaxHealth());
}

void UEosAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEosAttributeSet, MaxHealth, OldMaxHealth);
}

void UEosAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEosAttributeSet, CurrentStamina, OldStamina);
	PS = Cast<AEosBasePlayerState>(GetOwningActor());
	if (PS && PS->IsA<AEosBasePlayerState>())
	{
		Char = Cast<AEosBasePlayerCharacter>(PS->EosAbilitySystemComponent->GetAvatarActor());
		if (!Char) return;
		UIComponent = Cast<UEosBaseUIComponent>(Char->GetComponentByClass(UEosBaseUIComponent::StaticClass()));
	}
	else
	{
		UIComponent = Cast<UEosBaseUIComponent>(GetOwningActor()->GetComponentByClass(UEosBaseUIComponent::StaticClass()));
	}
	if (!UIComponent) return;
	UIComponent->OnStaminaChanged.Broadcast(GetCurrentStamina() / GetMaxStamina());
}

void UEosAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEosAttributeSet, MaxStamina, OldMaxStamina);
}

void UEosAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEosAttributeSet, CurrentMana, OldMana);
	PS = Cast<AEosBasePlayerState>(GetOwningActor());
	if (PS && PS->IsA<AEosBasePlayerState>())
	{
		Char = Cast<AEosBasePlayerCharacter>(PS->EosAbilitySystemComponent->GetAvatarActor());
		if (!Char) return;
		UIComponent = Cast<UEosBaseUIComponent>(Char->GetComponentByClass(UEosBaseUIComponent::StaticClass()));
	}
	else
	{
		UIComponent = Cast<UEosBaseUIComponent>(GetOwningActor()->GetComponentByClass(UEosBaseUIComponent::StaticClass()));
	}
	if (!UIComponent) return;
	UIComponent->OnManaChanged.Broadcast(GetCurrentMana() / GetMaxMana());
}

void UEosAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEosAttributeSet, MaxMana, OldMaxMana);
}


void UEosAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UEosAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEosAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEosAttributeSet, CurrentStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEosAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEosAttributeSet, CurrentMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEosAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

}