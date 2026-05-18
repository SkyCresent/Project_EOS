// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/EosBasePlayerController.h"
#include "Components/UI/EosPlayerUIComponent.h"
#include "CoreSystem/GameMode/EosVillageGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "EosGameInstance.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "Net/UnrealNetwork.h"
#include "CoreSystem/GameState/EosBaseGameState.h"
#include "DataAssets/Input/EosDataAsset_InputConfig.h"
#include "Components/Input/EosEnhancedInputComponent.h"
#include "HUD/EosVillageHUD.h"
#include "EnhancedInputSubsystems.h"
#include "EosGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CoreSystem/PlayerState/EosBasePlayerState.h"
#include "Character/NonePlayerCharacter/EosBaseNonePlayerCharacter.h"
#include "AbilitySystem/EosAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EosPlayerCombatComponent.h"
#include "Items/Weapon/EosBasePlayerWeapon.h"
#include "EosFunctionLibrary.h"

AEosBasePlayerController::AEosBasePlayerController()
{
	bReplicates = true;
	// 팀 설정
	PlayerTeamId = FGenericTeamId(0);
	// GM 캐스팅
	GameMode = Cast<AEosBaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AEosBasePlayerController::SetupInputComponent()
{
	// 인풋 바인딩
	checkf(InputConfigDataAssets, TEXT("Character DataAssets has null. Check your Character Data."))
	Super::SetupInputComponent();

	if (!IsLocalController()) return;

	ULocalPlayer* LocalPlayer = GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(SubSystem);

	SubSystem->AddMappingContext(InputConfigDataAssets->DefaultMappingContext, 0);

	UEosEnhancedInputComponent* PlayerEnhancedInputComponent = CastChecked<UEosEnhancedInputComponent>(InputComponent);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAssets, EosGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAssets, EosGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAssets, EosGameplayTags::InputTag_Interact, ETriggerEvent::Triggered, this, &ThisClass::Input_Interact);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAssets, EosGameplayTags::InputTag_Inventory, ETriggerEvent::Triggered, this, &ThisClass::Input_ToggleInventory);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAssets, EosGameplayTags::InputTag_FocusOnChat, ETriggerEvent::Triggered, this, &ThisClass::Input_FocusChat);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAssets, EosGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	PlayerEnhancedInputComponent->BindNativeInputAction(InputConfigDataAssets, EosGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);

	PlayerEnhancedInputComponent->BindAbilityInputAction(InputConfigDataAssets, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

FGenericTeamId AEosBasePlayerController::GetGenericTeamId() const
{
	// 팀 반환
	return PlayerTeamId;
}

void AEosBasePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEosBasePlayerController, GUID);
	DOREPLIFETIME(AEosBasePlayerController, bIsRaid);
}

void AEosBasePlayerController::Client_AddSelf_Implementation(const FPlayerInfo PlayerInfo)
{
	AEosBaseGameState* GameState = Cast<AEosBaseGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->AddParty(PlayerInfo);
	}
}

void AEosBasePlayerController::Client_UpdatePartyUI_Implementation(const FGuid TargetGUID, float NewHP)
{
	if (GetPawn())
	{
		UEosPlayerUIComponent* UIComponent = Cast<UEosPlayerUIComponent>(GetPawn()->GetComponentByClass(UEosPlayerUIComponent::StaticClass()));
		if (UIComponent)
		{
			UIComponent->OnUpdatePartyHP.Broadcast(TargetGUID, NewHP);
		}
	}
}

void AEosBasePlayerController::Client_SetPartyUI_Implementation(const TArray<FPlayerInfo>& PartyArr)
{
	for (auto Party : PartyArr)
	{
		if (Party.PlayerGUID != this->GUID)
		{
			if (GetPawn())
			{
				UEosPlayerUIComponent* UIComponent = Cast<UEosPlayerUIComponent>(GetPawn()->GetComponentByClass(UEosPlayerUIComponent::StaticClass()));
				if (UIComponent)
				{
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, Party.PlayerGUID.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, this->GUID.ToString());

					UIComponent->OnUpdateParty.Broadcast(Party);
				}
			}
		}
	}
}

void AEosBasePlayerController::Server_SendMessage_Implementation(const FText& msg)
{
	if (HasAuthority())
	{
		if (GameMode)
		{

			GameMode->UpdateMessage(msg);
		}
	}
}

void AEosBasePlayerController::Client_SendMessage_Implementation(const FText& msg)
{
	UEosPlayerUIComponent* UIComponent = Cast<UEosPlayerUIComponent>(GetPawn()->GetComponentByClass(UEosPlayerUIComponent::StaticClass()));
	if (UIComponent)
	{
		UIComponent->OnUpdateMessage.Broadcast(msg);
	}
}

void AEosBasePlayerController::Server_AskParty_Implementation(const FString& TargetNickName)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *this->GetGuid().ToString());

	Cast<AEosVillageGameMode>(GameMode)->CreateParty(this->GetGuid(), TargetNickName);
}

void AEosBasePlayerController::Client_AskParty_Implementation(const FGuid& HostGUID, const FString& HostNickName)
{
	UEosPlayerUIComponent* UIComponent = Cast<UEosPlayerUIComponent>(GetPawn()->GetComponentByClass(UEosPlayerUIComponent::StaticClass()));
	if (UIComponent)
	{
		UIComponent->OnAskParty.Broadcast(HostGUID, HostNickName);
	}
}

void AEosBasePlayerController::Client_AnswerParty_Implementation(const FGuid& HostGUID, const bool bIsParty)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, this->GetGuid().ToString());
	 
	this->Server_AnswerParty(HostGUID, this->GetGuid(), bIsParty);
}

void AEosBasePlayerController::Server_AnswerParty_Implementation(const FGuid& HostGUID, const FGuid& TargetGUID, const bool bIsParty)
{
	UE_LOG(LogTemp, Warning, TEXT("Server_AnswerParty"));
	if(GameMode)
		Cast<AEosVillageGameMode>(GameMode)->AnswerParty(HostGUID, TargetGUID, bIsParty);
	else
		UE_LOG(LogTemp, Warning, TEXT("No GameMode"));
}

FString AEosBasePlayerController::GetHostIP()
{
	FString LocalIP = "";
	bool bCanBind;
	TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);
	if (Addr->IsValid())
	{
		LocalIP = Addr->ToString(false); // 포트 제외 
	}
	return LocalIP;
}

AEosVillageHUD* AEosBasePlayerController::GetVillageHUD()
{
	if (GetHUD())
	{
		return Cast<AEosVillageHUD>(GetHUD());
	}
	return nullptr;
}

void AEosBasePlayerController::ToggleUIMode(bool EnabledMenu, UUserWidget* FocusWidget)
{
	if (EnabledMenu)
	{
		GetEosBasePlayerCharacter()->GetCharacterMovement()->StopMovementImmediately();
		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(FocusWidget->TakeWidget());				  
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);  
		SetInputMode(Mode);
		bShowMouseCursor = true;
	}
	else
	{
		FInputModeGameOnly Mode;
		SetInputMode(Mode);
		bShowMouseCursor = false;
	}
}

void AEosBasePlayerController::Server_ReportHostIP_Implementation(const FString& HostIP)
{
	bIsStartRaid = false;
	Cast<AEosVillageGameMode>(GameMode)->StartRaid(this->GetGuid(), HostIP);
}

void AEosBasePlayerController::Client_StartRaid_Implementation(const FString& HostIP)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, HostIP);
	//FString Port = ":7788";
	//FString Port = ":7779";
	ClientTravel(HostIP, TRAVEL_Absolute);
	//UGameplayStatics::OpenLevel(GetWorld(), FName::ToString(HostIP), true);

	//ClientTravel(HostIP+Port, TRAVEL_Absolute);
}

void AEosBasePlayerController::Client_SetRaid_Implementation()
{
	AEosBaseGameState* GameState = Cast<AEosBaseGameState>(GetWorld()->GetGameState());
	UEosGameInstance* GameInstance = Cast<UEosGameInstance>(GetGameInstance());
	if (GameState && GameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("GameState : %d"), GameState->GetPartyList().Num()));

		GameInstance->SetPartyList(GameState->GetPartyList());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "No GameState or GameInstance");

	}
	GetWorld()->GetTimerManager().SetTimer(OpenRaidTimerHandle, this, &AEosBasePlayerController::OpenRaid, 0.1f, false);
}

void AEosBasePlayerController::Server_CanStartRaid_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("CanStartRaud"));
	if (!bIsStartRaid)
	{
		bIsStartRaid = true;
		Cast<AEosVillageGameMode>(GameMode)->CanStartRaid(this->GetGuid());
	}
}

void AEosBasePlayerController::Client_AddPartyList_Implementation(FPlayerInfo NewParty)
{
	AEosBaseGameState* GameState = Cast<AEosBaseGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->AddParty(NewParty);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "AddPartyList");
	}
}

void AEosBasePlayerController::OpenRaid()
{
	//UGameplayStatics::OpenLevel(GetWorld(), FName("Raid_LionKnight"), true, "?listen");
	FString Level = "Raid_LionKnight";
	FString Option = "?listen";
	UEosGameInstance* GameInstance = Cast<UEosGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->MoveLevel(Level, Option);
	}

	Server_ReportHostIP(GetHostIP());
}

void AEosBasePlayerController::SwitchMenuMappingContext()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(SubSystem);

	AEosBasePlayerCharacter* PlayerChar = Cast<AEosBasePlayerCharacter>(GetCharacter());
	if (!PlayerChar) return;

	if (UEosFunctionLibrary::NativeDoesActorHaveTag(PlayerChar, EosGameplayTags::Player_Status_Weapon_Equip))
	{
		EPlayerClassType ClassType = PlayerChar->PlayerClassType;
		FGameplayTag Tag;
		AEosBasePlayerWeapon* CurrentWeapon = nullptr;
		switch (ClassType)
		{
		case EPlayerClassType::Empty:
			break;
		case EPlayerClassType::GreatSword:
			CurrentWeapon = GetCurrentWeapon(PlayerChar, EosGameplayTags::Player_Weapon_GreatSword);
			break;
		case EPlayerClassType::Healer:
			CurrentWeapon = GetCurrentWeapon(PlayerChar, EosGameplayTags::Player_Weapon_Staff);
			break;
		case EPlayerClassType::Lancer:
			CurrentWeapon = GetCurrentWeapon(PlayerChar, EosGameplayTags::Player_Weapon_Lance);
		case EPlayerClassType::Mage:
			break;
		default:
			break;
		}
		if(CurrentWeapon)
		{
			SubSystem->RemoveMappingContext(InputConfigDataAssets->DefaultMappingContext);
			SubSystem->RemoveMappingContext(CurrentWeapon->PlayerWeaponData.WeaponInputMappingContext);
			SubSystem->AddMappingContext(MenuMappingContext, 3);
		}
	}
	else
	{
		SubSystem->AddMappingContext(MenuMappingContext, 3);
		SubSystem->RemoveMappingContext(InputConfigDataAssets->DefaultMappingContext);
	}
}

void AEosBasePlayerController::SwitchIngameMappingContext()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(SubSystem);

	AEosBasePlayerCharacter* PlayerChar = Cast<AEosBasePlayerCharacter>(GetCharacter());
	if (!PlayerChar) return;

	if (UEosFunctionLibrary::NativeDoesActorHaveTag(PlayerChar, EosGameplayTags::Player_Status_Weapon_Equip))
	{
		EPlayerClassType ClassType = PlayerChar->PlayerClassType;
		FGameplayTag Tag;
		AEosBasePlayerWeapon* CurrentWeapon = nullptr;
		switch (ClassType)
		{
		case EPlayerClassType::Empty:
			break;
		case EPlayerClassType::GreatSword:
			CurrentWeapon = GetCurrentWeapon(PlayerChar, EosGameplayTags::Player_Weapon_GreatSword);
			break;
		case EPlayerClassType::Healer:
			CurrentWeapon = GetCurrentWeapon(PlayerChar, EosGameplayTags::Player_Weapon_Staff);
			break;
		case EPlayerClassType::Lancer:
			CurrentWeapon = GetCurrentWeapon(PlayerChar, EosGameplayTags::Player_Weapon_Lance);
			break;
		case EPlayerClassType::Mage:
			break;
		default:
			break;
		}
		if (CurrentWeapon)
		{
			SubSystem->AddMappingContext(InputConfigDataAssets->DefaultMappingContext,0);
			SubSystem->AddMappingContext(CurrentWeapon->PlayerWeaponData.WeaponInputMappingContext,1);
			SubSystem->RemoveMappingContext(MenuMappingContext);
		}
	}
	else
	{
		SubSystem->AddMappingContext(InputConfigDataAssets->DefaultMappingContext, 0);
		SubSystem->RemoveMappingContext(MenuMappingContext);
	}
}

void AEosBasePlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	// 속도
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, GetControlRotation().Yaw, 0.f);

	// 속력이 0이 아니면...
	if(APawn* PlayerPawn = GetPawn())
	{
		if (MovementVector.Y != 0.f)
		{
			// 전방 벡터 주입(앞,뒤)
			const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			PlayerPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		}
		if (MovementVector.X != 0.f)
		{
			// 우측 벡터 주입(좌,우)
			const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
			PlayerPawn->AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void AEosBasePlayerController::Input_Look(const FInputActionValue& InputActionValue)
{
	// 마우스 시점 입력
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	if (APawn* PlayerPawn = GetPawn())
	{
		if (LookAxisVector.X != 0.f)
		{
			PlayerPawn->AddControllerYawInput(LookAxisVector.X);
		}
		if (LookAxisVector.Y != 0.f)
		{
			PlayerPawn->AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

void AEosBasePlayerController::Input_Interact(const FInputActionValue& InputActionValue)
{
	//PlayerInventoryComponent->Interaction();

	// NPC - Player 상호작용

	FVector Start = GetPawn()->GetActorLocation();
	FVector End = Start;

	FHitResult Hit;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(50.f);

	bool bHit = GetWorld()->SweepSingleByObjectType(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel3,
		Sphere
	);

	if (bHit && Hit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Nearby NPC: %s"), *Hit.GetActor()->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Nearby NPC: %s"), *Hit.GetActor()->GetName()));

		AEosVillageHUD* VillageHUD = GetVillageHUD();

		if (VillageHUD)
		{
			AEosBaseNonePlayerCharacter* NonePlayerCharacter = Cast<AEosBaseNonePlayerCharacter>(Hit.GetActor());
			if (NonePlayerCharacter)
			{
				if(GetEosBasePlayerCharacter())
				{
					SetViewTargetWithBlend(NonePlayerCharacter, 0.5f);
					GetEosBasePlayerCharacter()->GetMesh()->SetVisibility(false, true);
					VillageHUD->ShowNPCDialog(NonePlayerCharacter->GetDialogID());
				}
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No HUD");
		}
	}

	DrawDebugSphere(GetWorld(), Start, 50.f, 12, FColor::Yellow, false, 2.0f);

}

void AEosBasePlayerController::Input_ToggleMenu(const FInputActionValue& InputActionValue)
{
	
}

void AEosBasePlayerController::Input_ToggleInventory(const FInputActionValue& InputActionValue)
{
	if (!IsLocalController()) return;

	if (UEosFunctionLibrary::NativeDoesActorHaveTag(GetCharacter(), EosGameplayTags::Player_Status_Dash) || UEosFunctionLibrary::NativeDoesActorHaveTag(GetCharacter(), EosGameplayTags::Player_Status_InCombat))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::MakeRandomColor(), TEXT("YOU CANNOT OPEN INVENTORY")); 
		return;
	}
	GetVillageHUD()->SwitchingInventory();
}

void AEosBasePlayerController::Input_ToggleSkill(const FInputActionValue& InputActionValue)
{

}

void AEosBasePlayerController::Input_ToggleMap(const FInputActionValue& InputActionValue)
{

}

void AEosBasePlayerController::Input_AbilityInputPressed(const FGameplayTag InInputTag)
{
	GetEosAbilitySystemComponent()->OnAbilityInputPressed(InInputTag);
}

void AEosBasePlayerController::Input_AbilityInputReleased(const FGameplayTag InInputTag)
{
	GetEosAbilitySystemComponent()->OnAbilityInputReleased(InInputTag);
}

void AEosBasePlayerController::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	//좌우 방향 벡터 수집
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void AEosBasePlayerController::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;
	// 방향 벡터를 토대로 좌/우 이벤트 발신
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor
	(
		this,
		SwitchDirection.X > 0.f ? EosGameplayTags::Player_Event_SwitchTarget_Right : EosGameplayTags::Player_Event_SwitchTarget_Left,
		Data
	);
}

void AEosBasePlayerController::Input_FocusChat()
{
	// 채팅 상호작용
	if(GetEosBasePlayerCharacter())
	{
		GetEosBasePlayerCharacter()->PlayerUIComponent->OnSendMessage.Broadcast();
	}
}

AEosBasePlayerWeapon* AEosBasePlayerController::GetCurrentWeapon(AEosBasePlayerCharacter* InActor, FGameplayTag TagToCheck) const
{
	return InActor->GetPlayerCombatComponent()->GetPlayerCarriedWeaponByTag(TagToCheck);
}
