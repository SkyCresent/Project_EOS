// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/EosPlayerGameplayAbility.h"
#include "EosPlayerGA_TargetLock.generated.h"

class UEosBaseWidget;
class AEosBasePlayerController;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosPlayerGA_TargetLock : public UEosPlayerGameplayAbility
{
	GENERATED_BODY()
///NET FUNCTION
protected:
	UFUNCTION(Server, Reliable)
	void Server_RequestRotationControl(AEosBasePlayerCharacter* InRotateCharacter, FRotator TargetRot);	
	
	UFUNCTION(Server, Reliable)
	void Server_RequestResetTargetLockMovement(UCharacterMovementComponent* InMovementComp);

	UFUNCTION(Client, Reliable)
	void Client_RequestRotationControl(AEosBasePlayerCharacter* InRotateCharacter, FRotator TargetRot);

	UFUNCTION(Server, Reliable)
	void Server_RequestInitTargetLockMovement(UCharacterMovementComponent* InMovementComp);

	UFUNCTION(Client, Reliable)
	void Client_RequestInitTargetLockMovement(UCharacterMovementComponent* InMovementComp);

	UFUNCTION(Server, Reliable)
	void Server_RequestResetDefaultWalkSpeed();

	UFUNCTION(Server, Reliable)
	void Server_RequestCheckEnemyStatus(AActor* CurrentTarget);

	UFUNCTION(Server, Reliable)
	void Server_RequestCheckEnemyStatusForTryLockOnTarget(AActor* CurrentTarget);
	UFUNCTION(Client, Reliable)
	void Client_RequestCheckEnemyStatusForTryLockOnTarget();

	UFUNCTION(Client, Reliable)
	void Client_RequestCheckEnemyStatus();


///ENGINE FUNCTION
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
///CUSTOM FUNCTION
public:
protected:
	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void SwitchTarget(const FGameplayTag& InSwitchDirectionTag);

private:
	void TryLockOnTarget();
	void LockOnTarget();
	void GetAvailableActorsToLock();
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);
	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsLeft, TArray<AActor*>& OutActorsRight);

	void SetTargetLockWidgetPosition();
	void DrawTargetLockWidget();
	void InitTargetLockMovement();
	void InitTargetLockContext();

	void CancelTargetLockAbility();
	void CleanUp();
	void ResetTargetLockMovement();
	void ResetTargetLockContext();

///CUSTOM VARIABLE	
protected:

private:

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock | Data")
	TSubclassOf<UEosBaseWidget> TargetLockWidgetClass;

	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;
	
	UPROPERTY(EditDefaultsOnly, Category = "TargetLock | Data")
	float BoxTraceDistance = 1500.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "TargetLock | Data")
	FVector TraceBoxSize = FVector(1500.f, 1500.f, 1500.f);

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock | Data")
	bool bShowDebugShape = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "TargetLock | Data")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

	UPROPERTY()
	UEosBaseWidget* TargetLockWidget;

	UPROPERTY()
	AActor* CurrentLockedActor;

	UPROPERTY()
	FVector2D TargetWidgetLockSize = FVector2D::ZeroVector;

	UPROPERTY()
	AEosBasePlayerController* PC;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock | Data")
	float CameraOffsetDistance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock | Data")
	float TargetLookRotationInterpSpeed = 5.f;

	UPROPERTY(BlueprintReadOnly, Replicated, meta = (AllowPrivateAccess = "true"))
	float CachedDefaultMaxWalkSpeed = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock | Data")
	float TargetLockMaxWalkSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock | Data")
	UInputMappingContext* TargetLockMappingContext;
};
