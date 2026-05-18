// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Struct/EosStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "EosBaseWeapon.generated.h"

class UBoxComponent;


UENUM()
enum class EWeaponEquipType : uint8
{
	RightHand,
	LeftHand,
	TwoHands
};

DECLARE_DELEGATE_OneParam(FOnTargetHitDelegate, AActor*)

UCLASS()
class PROJECTEOS_API AEosBaseWeapon : public AActor
{
	GENERATED_BODY()


/// ENGINE FUNCTION
public:	
	// Sets default values for this actor's properties
	AEosBaseWeapon();

	//TODO
	FOnTargetHitDelegate OnWepaonHitTarget;
	FOnTargetHitDelegate OnWeaponEndHitFromTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	virtual void OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



/// CUSTOM `
public:
	
protected:

private:




/// CUSTOM VARIABLE

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipType")
	bool bIsParticleWeapon = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipType")
	EWeaponEquipType WeaponEquipType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ParticleWeapon")
	UParticleSystem* ParticleWeaponL;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ParticleWeapon")
	UParticleSystem* ParticleWeaponR;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SubWeapons")
	UStaticMeshComponent* SubWeaponMesh;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UBoxComponent* WeaponCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons")
	UBoxComponent* SubWeaponCollisionBox;

private:
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;

/// GETTER FUNCTION
public:

	FORCEINLINE UBoxComponent* GetWeaponCollisionBox() const
	{
		return WeaponCollisionBox;
	}
	
	FORCEINLINE UBoxComponent* GetSubWeaponCollisionBox() const
	{
		return SubWeaponCollisionBox;
	}
};