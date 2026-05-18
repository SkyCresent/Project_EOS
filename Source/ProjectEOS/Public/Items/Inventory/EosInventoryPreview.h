// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EosInventoryPreview.generated.h"

//class USceneCaptureComponent2D;
class USceneComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class AEosBasePlayerCharacter;

UCLASS()
class PROJECTEOS_API AEosInventoryPreview : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEosInventoryPreview();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EnterInventoryMode(AEosBasePlayerCharacter* InChar);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ExitInventoryMode(AEosBasePlayerCharacter* InChar);

public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Showcase")
	//USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Showcase")
	UStaticMeshComponent* FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Showcase")
	USceneComponent* PlayerPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Showcase")
	USceneComponent* PlayerMeshPos;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Showcase")
	USpringArmComponent* MainArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Showcase")
	UCameraComponent* Camera;

	//UPROPERTY(VisibleAnywhere, Category = "Showcase")
	//USkeletalMeshComponent* PreviewMeshComponent;
protected:
	FTransform CurrentMeshTransform;
	
};
