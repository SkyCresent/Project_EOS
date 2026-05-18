// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc/GeExecCalc_DamageTaken.h"
#include "AbilitySystem/EosAttributeSet.h"
#include "EosGameplayTags.h"

// 계산에 사용할 속성 캡쳐 정의
struct FEosDamageCapture
{
	// 선언
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FEosDamageCapture()
	{
		//정의 Class, Capture, Target, Snapshot 등 메타 정보만 기록
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEosAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEosAttributeSet, DefensePower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UEosAttributeSet, DamageTaken, Target, false)
	}
};

// 캡처를 최초 호출 시 한 번만 생성
// 상단에 DEFINE_ATTRIBUTE_CAPTUREDEF 매크로로 정의된 최초의 메타 데이터를 재활용
static const FEosDamageCapture& GetEosDamageCapture()
{
	static FEosDamageCapture EosDamageCapture;
	return EosDamageCapture;
}

// 
UGeExecCalc_DamageTaken::UGeExecCalc_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetEosDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetEosDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetEosDamageCapture().DamageTakenDef);
}

// 이펙트 실행 시
void UGeExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetEosDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetEosDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);

	// 사용할 변수를 선언
	float BaseDamage = 0.f;
	float SkillDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedSmashAttackComboCount = 0;
	
	// SetByCaller로 받은 맵을 페어로 분리
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		// 키의 종류에 따라서 변수에 값 전달
		if (TagMagnitude.Key.MatchesTagExact(EosGameplayTags::Common_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}
		if (TagMagnitude.Key.MatchesTagExact(EosGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
		}
		if (TagMagnitude.Key.MatchesTagExact(EosGameplayTags::Player_SetByCaller_AttackType_Smash))
		{
			UsedSmashAttackComboCount = TagMagnitude.Value;
		}
		if (TagMagnitude.Key.MatchesTagExact(EosGameplayTags::Player_SetByCaller_AttackType_Skill))
		{
			SkillDamage = TagMagnitude.Value;
		}
	}

	// 이하 조건에 따른 연산
	if (UsedLightAttackComboCount != 0)
	{
		//평타 2타부터 대미지 5% 증감
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.05 + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
	}
	//스매시 카운트당 대미지 15% 증감 (0타 스매시 구현의 경우 8% 증감)
	/*if (UsedSmashAttackComboCount == 0)
	{
		const float DamageIncreasePercentSmash = 1 * 0.8f + 1.f;

		BaseDamage *= DamageIncreasePercentSmash;
	}*/
	if (UsedSmashAttackComboCount != 0)
	{
		const float DamageIncreasePercentSmash = UsedSmashAttackComboCount * 0.15f + 1.f;

		BaseDamage *= DamageIncreasePercentSmash;
	}

	//최종 연산RE
	const float FinalDamage = BaseDamage * SourceAttackPower / TargetDefensePower;

	if (FinalDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier
		(
			FGameplayModifierEvaluatedData
			(
				GetEosDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamage
			)
		);
	}
}