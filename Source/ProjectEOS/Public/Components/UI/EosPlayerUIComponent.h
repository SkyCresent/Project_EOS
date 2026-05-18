// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/EosBaseUIComponent.h"
#include "EosPlayerUIComponent.generated.h"

class AEosBasePlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateMessage, const FText&, InMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemMessage, const FText&, InMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSendMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateParty, const FPlayerInfo&, InPlayerInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdatePartyHP, const FGuid, TargetGUID, float, HP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnServerUpdatePartyHP, const FGuid, TargetGUID, float, HP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAskParty, const FGuid, HostGUID, const FString&, InHostNickName);

/**
 *
 */
UCLASS()
class PROJECTEOS_API UEosPlayerUIComponent : public UEosBaseUIComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Message")
	FOnUpdateMessage OnUpdateMessage;

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FOnSendMessage OnSendMessage;

	UPROPERTY(BlueprintAssignable, Category = "Party")
	FOnUpdateParty OnUpdateParty;
	
	UPROPERTY(BlueprintAssignable, Category = "Party")
	FOnUpdatePartyHP OnUpdatePartyHP;

	UPROPERTY(BlueprintAssignable, Category = "Party")
	FOnServerUpdatePartyHP OnServerUpdatePartyHP;

	UPROPERTY(BlueprintAssignable, Category = "Party")
	FOnAskParty OnAskParty;

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FOnSystemMessage OnSystemMessage;

};
