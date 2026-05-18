// Fill out your copyright notice in the Description page of Project Settings.


#include "EosFunctionLibrary.h"
#include "AbilitySystem/EosAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/Combat/EosBaseCombatComponent.h"
#include "Interfaces/EosCombatInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "EosGameplayTags.h"
#include "GenericTeamAgentInterface.h"
#include "Engine/Engine.h"

UEosAbilitySystemComponent* UEosFunctionLibrary::GetNativeEosAbilitySystemComponent(AActor* InActor)
{
	if (!ensureMsgf(InActor, TEXT("InActor is not valid from GetNativeEosAbilitySystemComponent()"))) return nullptr;

	return Cast<UEosAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UEosFunctionLibrary::RemoveGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UEosAbilitySystemComponent* ASC = GetNativeEosAbilitySystemComponent(InActor);

	if (!ensureMsgf(ASC, TEXT("InActor is not valid from GetNativeEosAbilitySystemComponent()"))) return;
	// 태그 삭제
	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

void UEosFunctionLibrary::RemoveReplicatedGameplayFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UEosAbilitySystemComponent* ASC = GetNativeEosAbilitySystemComponent(InActor);

	if (!ensureMsgf(ASC, TEXT("InActor is not valid from GetNativeEosAbilitySystemComponent()"))) return;
	// 태그 삭제
	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveReplicatedLooseGameplayTag(TagToRemove);
	}
}

void UEosFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UEosAbilitySystemComponent* ASC = GetNativeEosAbilitySystemComponent(InActor);
	
	if (!ensureMsgf(ASC, TEXT("InActor is not valid from GetNativeEosAbilitySystemComponent()"))) return;
	// 태그 추가
	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UEosFunctionLibrary::AddReplicatedGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UEosAbilitySystemComponent* ASC = GetNativeEosAbilitySystemComponent(InActor);

	if (!ensureMsgf(ASC, TEXT("InActor is not valid from GetNativeEosAbilitySystemComponent()"))) return;
	// 태그 추가
	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddReplicatedLooseGameplayTag(TagToAdd);
	}
}

bool UEosFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UEosAbilitySystemComponent* ASC = GetNativeEosAbilitySystemComponent(InActor);
	// 태그 소유 점검
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UEosFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EEosConfirmType& OutConfirmType)
{
	// 태그 소유 점검 요청 및 유효 여부 반환
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EEosConfirmType::Yes : EEosConfirmType::No;
}

UEosBaseCombatComponent* UEosFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	//check(InActor);

	if(IEosCombatInterface* BaseCombatInterface = Cast<IEosCombatInterface>(InActor))
	{
		// 전투 컴포넌트 반환
		return BaseCombatInterface->GetPawnCombatComponent();
	}
	return nullptr;
}

UEosBaseCombatComponent* UEosFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EEosValidType& OutValidType)
{
	// 전투 컴포넌트 반환 요청
	UEosBaseCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	// 유효 여부 반환
	OutValidType = CombatComponent ? EEosValidType::Valid : EEosValidType::Invalid;
	
	return CombatComponent;
}

float UEosFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFlaot, float InLevel)
{
	// 레벨에 대한 ScalableFloat 반환
	return InScalableFlaot.GetValueAtLevel(InLevel);
}

bool UEosFunctionLibrary::IsListenServer(UObject* WorldContextObject)
{
	// 리슨 서버 여부 반환
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		return World->GetNetMode() == NM_ListenServer;
	}
	return false;
}

bool UEosFunctionLibrary::IsValidGuard(AActor* InAttacker, AActor* InDefender)
{
	check(InAttacker && InDefender);

	// 공격자와 방어자의 전방벡터를 점곱
	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());
	
	// 디버깅
	const FString GuardValid = FString::Printf(TEXT("Dot Result: %f, %s"), DotResult, DotResult < 0.f ? TEXT("Guard") : TEXT("Failed Guard"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, DotResult < 0.f ? FColor::Green : FColor::Red, GuardValid);

	// 마주보고 있으면 true
	return DotResult < -0.1f;
}

bool UEosFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UEosAbilitySystemComponent* SourceASC = GetNativeEosAbilitySystemComponent(InInstigator);
	UEosAbilitySystemComponent* TargetASC = GetNativeEosAbilitySystemComponent(InTargetActor);

	// 스펙 핸들로 타겟에 이펙트 발동하고 핸들 보관
	FActiveGameplayEffectHandle ApplyGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

	// 이펙트 적용 여부 반환
	return ApplyGameplayEffectHandle.WasSuccessfullyApplied();
}

FGameplayTag UEosFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InReacter, float& OutAngleDiffrence)
{
	check(InAttacker && InReacter);
	// 대상자 전방 벡터
	const FVector ReacterForward = InReacter->GetActorForwardVector();
	// 공격자 위치로 향하는 단위 벡터 계산
	const FVector ReacterNormalized = (InAttacker->GetActorLocation() - ReacterForward).GetSafeNormal();
	
	// 내적 계산
	const float DotResult = FVector::DotProduct(ReacterForward, ReacterNormalized);
	// 각도 환산
	OutAngleDiffrence = UKismetMathLibrary::DegAcos(DotResult);
	// 외적 계산
	const FVector CrossResult = FVector::CrossProduct(ReacterForward, ReacterNormalized);

	if (CrossResult.Z < 0.f)
	{
		// 왼쪽이 기준일 경우
		OutAngleDiffrence *= -1.f;
	}

	if (OutAngleDiffrence >= -45.f && OutAngleDiffrence < 45.f)
	{
		return EosGameplayTags::Common_Status_HitReact_Front;
	}
	else if (OutAngleDiffrence < -45.f && OutAngleDiffrence >= -135.f)
	{
		return EosGameplayTags::Common_Status_HitReact_Left;
	}
	else if (OutAngleDiffrence > 45.f || OutAngleDiffrence <= 135.f)
	{
		return EosGameplayTags::Common_Status_HitReact_RIght;
	}
	else if (OutAngleDiffrence < -135.f || OutAngleDiffrence > 135.f)
	{
		return EosGameplayTags::Common_Status_HitReact_Back;
	}

	return EosGameplayTags::Common_Status_HitReact_Front;
}

bool UEosFunctionLibrary::CheckTargetIsEnemy(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);

	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());
	// 팀 여부 반환
	if (QueryTeamAgent && TargetTeamAgent)
	{
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	return false;
}

bool UEosFunctionLibrary::CheckTextLength(const FString InString, int32 MinLength, int32 MaxLength)
{
	if (InString.Len() > MinLength && InString.Len() <= MaxLength)
		return true;
	return false;
}
