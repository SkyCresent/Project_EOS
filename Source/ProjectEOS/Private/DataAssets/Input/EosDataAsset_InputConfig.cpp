// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Input/EosDataAsset_InputConfig.h"
// 검증 요청된 태그가 배열에 있는 지 확인
UInputAction* UEosDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const FEosInputActionConfig& InputActionConfig : NativeInputActions)
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			// 있으면 IA 반환
			return InputActionConfig.InputAction;
		}
	return nullptr;
}
