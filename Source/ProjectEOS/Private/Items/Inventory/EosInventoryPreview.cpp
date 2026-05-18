// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Inventory/EosInventoryPreview.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/PlayerCharacter/EosBasePlayerCharacter.h"
#include "PlayerController/EosBasePlayerController.h"

// Sets default values
AEosInventoryPreview::AEosInventoryPreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
    FloorMesh->SetupAttachment(Root);
    FloorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    PlayerPos = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerPos"));
    PlayerPos->SetupAttachment(Root);

    MainArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    MainArm->SetupAttachment(PlayerPos);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(MainArm);

    PlayerMeshPos = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerMeshPos"));
    PlayerMeshPos->SetupAttachment(PlayerPos);

    //SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    //SceneCaptureComponent->SetupAttachment(Root);
    //SceneCaptureComponent->bCaptureEveryFrame = true;
    //SceneCaptureComponent->bCaptureOnMovement = false;

    //PreviewMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Dummy"));
    //PreviewMeshComponent->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AEosInventoryPreview::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEosInventoryPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEosInventoryPreview::EnterInventoryMode(AEosBasePlayerCharacter* InChar)
{
    if (!InChar) return;

    CurrentMeshTransform = InChar->GetMesh()->GetRelativeTransform();
    FVector MeshPosLoc = PlayerMeshPos->GetComponentTransform().GetLocation();
    FQuat MeshPosRot = PlayerMeshPos->GetComponentTransform().GetRotation();

    InChar->GetMesh()->SetWorldLocationAndRotation(MeshPosLoc, MeshPosRot);

    InChar->GetEosPlayerController()->SetViewTargetWithBlend(this);
}

void AEosInventoryPreview::ExitInventoryMode(AEosBasePlayerCharacter* InChar)
{
    if (!InChar) return;

    InChar->GetMesh()->SetRelativeLocationAndRotation(CurrentMeshTransform.GetLocation(), CurrentMeshTransform.GetRotation());

    InChar->GetEosPlayerController()->SetViewTargetWithBlend(InChar);
}