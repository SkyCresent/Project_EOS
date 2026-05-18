// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/EosBasePlayerController.h"
#include "EosWorldPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEOS_API AEosWorldPlayerController : public AEosBasePlayerController
{
	GENERATED_BODY()

/// NET FUNCTION
public:
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_GetGameInstance();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_GetGameInstance(FPlayerInfo PlayerInfo);

	UFUNCTION(Client, Reliable)
	void Client_SystemMessage(const FText& InMessage);

};
