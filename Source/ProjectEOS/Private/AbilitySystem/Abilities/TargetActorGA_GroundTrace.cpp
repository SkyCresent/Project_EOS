// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TargetActorGA_GroundTrace.h"

// 타겟팅 시작 시 호출 : Ability와 컨트롤러 설정 + 리타클(Decal) 생성
void ATargetActorGA_GroundTrace::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);
    OwningAbility = Ability;
    MyReticleActor = SpawnReticleActor(GetActorLocation(), GetActorRotation());
    UE_LOG(LogTemp, Warning, TEXT("Reticle Spawn Attempt: %s"), *GetNameSafe(CustomReticleClass));
}

// 매 프레임 호출되는 라인트레이스 함수
bool ATargetActorGA_GroundTrace::LineTraceFN(FHitResult& OutHit)
{
    FVector Start;
    // Character(PrimaryPC) 또는 AI(MasterAIController)에서 시작 위치 결정
    if (PrimaryPC && PrimaryPC->GetPawn())
    {
        Start = PrimaryPC->GetPawn()->GetActorLocation();
    }
    else if (MasterAIController && MasterAIController->GetPawn())
    {
        Start = MasterAIController->GetPawn()->GetActorLocation();
    }
    else return false;

    // 시작 위치에서 아래로 MaxTargetDistance만큼 쏘기
    FVector End = Start - FVector(0.f, 0.f, MaxTargetDistance);
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 2.0f);
    
    // 지면을 향해 라인트레이스 수행
    return GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility);
}

// 타겟팅 확정 시 호출 : TargetData 생성 후 브로드캐스트
void ATargetActorGA_GroundTrace::ConfirmTargetingAndContinue()
{
    FHitResult Hit;
    if (LineTraceFN(Hit))
    {
        FGameplayAbilityTargetDataHandle Data;
        Data.Add(new FGameplayAbilityTargetData_SingleTargetHit(Hit));
        TargetDataReadyDelegate.Broadcast(Data);
    }
    else
    {
        TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
    }
    DestroyReticleActors();
}
