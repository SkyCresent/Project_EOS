// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "EosDataAsset_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;


/**
 * 
 */
USTRUCT(BlueprintType)
struct FEosInputActionConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;

/// HELPER FUNCTION
	bool IsValid() const
	{
		return InputTag.IsValid() && InputAction;
	}

	// 기본 생성자
	FEosInputActionConfig()
		: InputTag(FGameplayTag())  // FGameplayTag는 기본 생성자로 초기화
		, InputAction(nullptr)       // UInputAction*은 nullptr로 초기화
	{}
};

UCLASS()
class PROJECTEOS_API UEosDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()

/// ENGINE FUNCTION


/// CUSTOM FUNCTION

public:
	// 기본 생성자
	UEosDataAsset_InputConfig()
		: DefaultMappingContext(nullptr)   // UInputMappingContext*는 nullptr로 초기화
		, NativeInputActions()             // TArray<FEosInputActionConfig>는 빈 배열로 초기화
		, AbilityInputActions()            // TArray<FEosInputActionConfig>는 빈 배열로 초기화
	{}
protected:

private:

/// CUSTOM VARIABLE

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FEosInputActionConfig> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FEosInputActionConfig> AbilityInputActions;

	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;


protected:

private:

	
};