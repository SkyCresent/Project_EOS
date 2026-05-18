// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NonePlayerCharacter/EosBaseNonePlayerCharacter.h"
#include "PlayerController/EosBasePlayerController.h"
#include "HUD/EosVillageHUD.h"
#include "Widget/NPC/EosNPCDialogWidget.h"

AEosBaseNonePlayerCharacter::AEosBaseNonePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	bReplicates = true;
}

void AEosBaseNonePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AEosBaseNonePlayerCharacter::InteractWith(AActor* InterActor)
{
	return;
}

FText AEosBaseNonePlayerCharacter::LookAt()
{
	return Name;
}

AEosBasePlayerController* AEosBaseNonePlayerCharacter::GetEosPlayerController(AActor* Actor) const
{
	ACharacter* Character;
	if (Actor)
	{
		Character = Cast<ACharacter>(Actor);
		if (Character)
		{
			AController* DefaultController = Character->GetController();
			if (DefaultController)
			{
				return Cast<AEosBasePlayerController>(DefaultController);
			}
		}
	}
	return nullptr;
}

FName AEosBaseNonePlayerCharacter::GetDialogID() const
{
	return DialogID;
}
