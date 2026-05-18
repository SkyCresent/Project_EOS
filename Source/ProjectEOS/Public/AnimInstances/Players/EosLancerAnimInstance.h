// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/EosCharacterAnimInstance.h"
#include "EosLancerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEOS_API UEosLancerAnimInstance : public UEosCharacterAnimInstance
{
	GENERATED_BODY()

	/// DEFAULT FUNCTION

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:

private:

	/// CUSTOM VARIABLE

};
