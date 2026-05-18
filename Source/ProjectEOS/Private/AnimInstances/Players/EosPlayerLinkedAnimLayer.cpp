// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Players/EosPlayerLinkedAnimLayer.h"

UEosCharacterAnimInstance* UEosPlayerLinkedAnimLayer::GetCharacterAnimInstance() const
{
	return Cast<UEosCharacterAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
