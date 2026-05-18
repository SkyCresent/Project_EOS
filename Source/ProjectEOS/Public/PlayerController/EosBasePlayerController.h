// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "Struct/EosPlayerInfo.h"
#include "Interfaces/EosHUDInterface.h"
#include "EosBasePlayerController.generated.h"

class AEosVillageHUD;
class UEosDataAsset_InputConfig;
class UInputMappingContext;
class AEosBasePlayerWeapon;

/**
 * 
 */
UCLASS()
class PROJECTEOS_API AEosBasePlayerController : public APlayerController, public IGenericTeamAgentInterface, public IEosHUDInterface
{
	GENERATED_BODY()


public:
	AEosBasePlayerController();

	virtual void SetupInputComponent() override;
	
	virtual FGenericTeamId GetGenericTeamId() const;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;




///CUSTOM FUNCTION
public:
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_AddSelf(const FPlayerInfo PlayerInfo);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_AnswerParty(const FGuid& HostGUID, const bool bIsParty);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_SendMessage(const FText& msg);

	UFUNCTION(Server, Reliable)
	void Server_SendMessage(const FText& msg);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_SetPartyUI(const TArray<FPlayerInfo>& PartyArr);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_UpdatePartyUI(const FGuid TargetGUID, float NewHP);

	UFUNCTION(Server, Reliable)
	void Server_AskParty(const FString& TargetNickName);

	UFUNCTION(Client, Reliable)
	void Client_AskParty(const FGuid& HostGUID, const FString& HostNickName);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_AnswerParty(const FGuid& HostGUID, const FGuid& TargetGUID, const bool bIsParty);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_AddPartyList(FPlayerInfo NewParty);

	UFUNCTION(Client, Reliable)
	void Client_SetRaid();

	UFUNCTION()
	void OpenRaid();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_ReportHostIP(const FString& HostIP);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_StartRaid(const FString& HostIP);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_CanStartRaid();

	UFUNCTION()
	FString GetHostIP();

	UFUNCTION(BlueprintPure)
	AEosVillageHUD* GetVillageHUD();

	virtual void ToggleUIMode(bool EnabledMenu, UUserWidget* FocusWidget) override;


///CUSTOM VARIABLE
public:
	UPROPERTY()
	FPlayerInfo CurrentPlayerInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player | Location", Replicated)
	bool bIsRaid = true;

protected:
	UPROPERTY()
	class AEosBaseGameMode* GameMode;

private:
	FGenericTeamId PlayerTeamId;

	UPROPERTY()
	FTimerHandle StartRaidTimerHandle;

	UPROPERTY(Replicated)
	FGuid GUID;

	UPROPERTY()
	FTimerHandle OpenRaidTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UEosDataAsset_InputConfig* InputConfigDataAssets;

	FVector2D SwitchDirection = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MenuMappingContext;

	bool bIsStartRaid = false;

public:
	UFUNCTION(BlueprintCallable)
	void SetGuid(FGuid NewGuid) { GUID = NewGuid;	}

	UFUNCTION(BlueprintCallable)
	FGuid GetGuid() { return GUID;}

	UFUNCTION()
	FPlayerInfo GetPlayerInfo() const { return CurrentPlayerInfo; }

	UFUNCTION(BlueprintCallable)
	void SetIsRaid(bool NewIsRaid) { bIsRaid = NewIsRaid; }

	void SwitchMenuMappingContext();
	void SwitchIngameMappingContext();

protected:

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Interact(const FInputActionValue& InputActionValue);
	void Input_ToggleMenu(const FInputActionValue& InputActionValue);
	void Input_ToggleInventory(const FInputActionValue& InputActionValue);
	void Input_ToggleSkill(const FInputActionValue& InputActionValue);
	void Input_ToggleMap(const FInputActionValue& InputActionValue);
	void Input_AbilityInputPressed(const FGameplayTag InInputTag);
	void Input_AbilityInputReleased(const FGameplayTag InInputTag);
	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);
	void Input_FocusChat();

	AEosBasePlayerWeapon* GetCurrentWeapon(AEosBasePlayerCharacter* InActor, FGameplayTag TagToCheck) const;

///GETTER FUNCTION

public:
		FORCEINLINE UEosAbilitySystemComponent* GetEosAbilitySystemComponent() const
		{
			AEosBasePlayerState* PS = GetPlayerState<AEosBasePlayerState>();
			if (PS)
			{
				return PS->EosAbilitySystemComponent;
			}
			return nullptr;
		}

		FORCEINLINE AEosBasePlayerCharacter* GetEosBasePlayerCharacter() const
		{
			AEosBasePlayerCharacter* LocalEosPlayerCharacter = Cast<AEosBasePlayerCharacter>(GetPawn());
			if (LocalEosPlayerCharacter)
			{
				return LocalEosPlayerCharacter;
			}
			return nullptr;
		}
};
