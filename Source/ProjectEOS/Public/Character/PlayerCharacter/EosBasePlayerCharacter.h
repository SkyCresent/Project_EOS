// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EosBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "ProjectEOS/Public/Interfaces/EosInteractableInterface.h"
#include "CoreSystem/PlayerState/EosBasePlayerState.h"
#include "EosBasePlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UEosPlayerCombatComponent;
struct FInputActionValue;
class UEosPlayerUIComponent;
class UEosAttributeSet;
class AEosBasePlayerController;

/**
 * 
 */

UENUM(BlueprintType)
enum class EPlayerClassType : uint8
{
	Empty,
	GreatSword,
	Healer,
	Lancer,
	Mage
};

UENUM(BlueprintType)
enum class EPlayerSkillType : uint8
{
	None,
	Mana,
	Stamina
};

UCLASS()
class PROJECTEOS_API AEosBasePlayerCharacter : public AEosBaseCharacter
{
	GENERATED_BODY()

	friend class AEosBasePlayerController;


/// NET FUNCTION
public:
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "EOS | Warp")
	void Server_AddOrUpdateWarpTargetfromLocationandRotation(const FName InName, FVector InTargetLocation, FRotator InTargetRotation);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "EOS | Warp")
	void Multicast_AddOrUpdateWarpTargetfromLocationandRotation(const FName InName, FVector InTargetLocation, FRotator InTargetRotation);

	UFUNCTION(Server, Reliable, Category = "EOS | Warp")
	void Server_AddOrUpdateWarpTargetfromLocation(const FVector InTargetLocation);

	UFUNCTION(NetMulticast, Reliable, Category = "EOS | Warp")
	void Multicast_AddOrUpdateWarpTargetfromLocation(const FVector InTargetLocation);

	UFUNCTION(Server, Reliable, Category = "EOS | Warp")
	void Server_RemoveWarpTarget(const FName InWarpTarget);

	UFUNCTION(NetMulticast, Reliable, Category = "EOS | Warp")
	void Multicast_RemoveWarpTarget(const FName InWarpTarget);

	UFUNCTION(NetMulticast, Reliable, Category = "EOS | WeaponAttachment")
	void Multicast_SpawnWeaponParticleL(USkeletalMeshComponent* InSkeletalMesh, AEosBaseWeapon* InFoundWeapon, const FName& InWeaponSocket);

	UFUNCTION(NetMulticast, Reliable, Category = "EOS | WeaponAttachment")
	void Multicast_SpawnWeaponParticleR(USkeletalMeshComponent* InSkeletalMesh, AEosBaseWeapon* InFoundWeapon, const FName& InWeaponSocket);
	
	UFUNCTION(NetMulticast, Reliable, Category = "EOS | WeaponAttachment")
	void Multicast_SpawnWeaponParticleSubR(USkeletalMeshComponent* InSkeletalMesh, AEosBaseWeapon* InFoundWeapon, const FName& InWeaponSocket);
	

/// ENGINE FUNCTION
public:
	AEosBasePlayerCharacter();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

/// CUSUTOM FUNCTION
public:
	UFUNCTION(BlueprintNativeEvent)
	void EndIntroBossScene();
	void BindIntroDelegate();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

/// CUSTOM FUNCTION
public:
	virtual UEosBaseCombatComponent* GetPawnCombatComponent() const override;

protected:
	virtual void OnCutSceneFinished() override;

/// CUSTOM VARIABLE
public:
	//Combo
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
	int32 CurrentComboCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	float ComboContinueTime = 0.3;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	FTimerHandle ComboContinueTimerHandle;

	UPROPERTY(BlueprintReadWrite, Category = "Gaurd")
	bool bIsGuarding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UEosPlayerUIComponent* PlayerUIComponent;

	//UPROPERTY(EditDefaultsOnly, Category = "Scene")
	//UAnimMontage* CinematicIdleAnim;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Policy", meta = (AllowPrivateAccess = "true"))
	EPlayerClassType PlayerClassType = EPlayerClassType::Empty;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UEosPlayerCombatComponent* PlayerCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	UEosAttributeSet* PlayerAttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "Reference", meta = (AllowPrivateAccess = "true"))
	AEosBasePlayerController* EosPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UEosAbilitySystemComponent* EosAbilitySystemComponent;

	FTimerHandle EndIntroSequenceHandle;

private:
	UPROPERTY(Replicated)
	EPlayerSkillType SkillType;


/// GETTER FUNCTION
public:
	FORCEINLINE UEosAbilitySystemComponent* GetEosAbilitySystemComponent() const override
	{
		AEosBasePlayerState* PS = Cast<AEosBasePlayerState>(GetPlayerState());
		if(PS)
		{
			return PS->EosAbilitySystemComponent;
		}
		return nullptr;
	}
	FORCEINLINE UEosAttributeSet* GetEosAttributeSet() const
	{
		AEosBasePlayerState* PS = Cast<AEosBasePlayerState>(GetPlayerState());
		if (PS)
		{
			return PS->EosAttributeSet;
		}
		return nullptr;
	}
	FORCEINLINE UEosPlayerCombatComponent* GetPlayerCombatComponent() const
	{
		return PlayerCombatComponent;
	}
	FORCEINLINE UCameraComponent* GetCameraComponent() const
	{
		return Camera;
	}
	FORCEINLINE UEosInventoryComponent* GetInventoryComponent() const
	{
		AEosBasePlayerState* PS = Cast<AEosBasePlayerState>(GetPlayerState());
		if (PS)
		{
			return PS->PlayerInventoryComponent;
		}
		return nullptr;
	}
	FORCEINLINE UEosPlayerUIComponent* GetPlayerUIComponent() const
	{
		return PlayerUIComponent;
	}
	AEosBasePlayerController* GetEosPlayerController();
/// GETTER FUNCTION : BLUEPRINT

	UFUNCTION(BlueprintPure, Category = "Player | Getter")
	UEosInventoryComponent* BP_GetInventoryComponent() const;

	UFUNCTION(BlueprintPure, Category = "Player | Getter")
	EPlayerSkillType GetPlayerSkillType() const;

	UFUNCTION()
	void SetPlayerSkillType(EPlayerSkillType NewSkillType);
};
