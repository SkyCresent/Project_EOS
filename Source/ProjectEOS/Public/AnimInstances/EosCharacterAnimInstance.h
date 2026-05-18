// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/EosBaseAnimInstance.h"
#include "EosCharacterAnimInstance.generated.h"

class AEosBaseCharacter;
class AEosBasePlayerCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosCharacterAnimInstance : public UEosBaseAnimInstance
{
	GENERATED_BODY()


/// DEFAULT FUNCTION

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation (float DeltaSeconds) override;

protected:

private:

/// CUSTOM VARIABLE
public:

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
	AEosBaseCharacter* OwningCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
	AEosBasePlayerCharacter* OwningPlayerCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimDataLocomotionData")
	float Speed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimDataLocomotionData")
	float Direction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimDataLocomotionData")
	bool bIsAcceleration;


private:

};