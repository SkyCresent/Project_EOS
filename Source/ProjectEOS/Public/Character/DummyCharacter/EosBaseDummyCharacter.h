// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EosBaseCharacter.h"
#include "EosBaseDummyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTEOS_API AEosBaseDummyCharacter : public AEosBaseCharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	

public:

};
