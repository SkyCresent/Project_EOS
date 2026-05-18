// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/EosWorldPlayerController.h"
#include "CoreSystem/GameMode/EosVillageGameMode.h"
#include "Components/UI/EosPlayerUIComponent.h"
#include "EosGameInstance.h"


void AEosWorldPlayerController::Client_SystemMessage_Implementation(const FText& InMessage)
{
	UEosPlayerUIComponent* UIComponent = Cast<UEosPlayerUIComponent>(GetPawn()->GetComponentByClass(UEosPlayerUIComponent::StaticClass()));
	if (UIComponent)
	{
		UIComponent->OnSystemMessage.Broadcast(InMessage);
	}
}

void AEosWorldPlayerController::Client_GetGameInstance_Implementation()
{
	if (!IsLocalPlayerController()) return;

	UEosGameInstance* GameInstance = Cast<UEosGameInstance>(GetGameInstance());

	if (GameInstance)
	{
		CurrentPlayerInfo = GameInstance->GetPlayerInfo();
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("%d"), CurrentPlayerInfo.CharacterClass));

		Server_GetGameInstance(CurrentPlayerInfo);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, CurrentPlayerInfo.PlayerGUID.ToString());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "No GameInstance");
	}
}

void AEosWorldPlayerController::Server_GetGameInstance_Implementation(FPlayerInfo PlayerInfo)
{
	if (GameMode)
	{
		GameMode->GetPlayerInfo(this, PlayerInfo);
	}
}
