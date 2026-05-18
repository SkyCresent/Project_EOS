// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TargetActorGA_SingleLineTrace.h"

// 타겟팅 시작 시 호출 : Ability와 컨트롤러 설정 + 리타클(Decal) 생성
void ATargetActorGA_SingleLineTrace::StartTargeting(UGameplayAbility* Ability)
{
    //부모에서 Character(PrimaryPC) 또는 AI(MasterAIController) 설정
    Super::StartTargeting(Ability);
    OwningAbility = Ability;
    MyReticleActor = SpawnReticleActor(GetActorLocation(), GetActorRotation());
    UE_LOG(LogTemp, Warning, TEXT("Reticle Spawn Attempt: %s"), *GetNameSafe(CustomReticleClass));
}

//카메라(또는 AI) 기준 직선 방향으로 라인트레이스
bool ATargetActorGA_SingleLineTrace::LineTraceFN(FHitResult& OutHit)
{
    FVector ViewPoint;
    FRotator ViewRot;

    // 플레이어 컨트롤러에서 시야 가져오기
    if (PrimaryPC) PrimaryPC->GetPlayerViewPoint(ViewPoint, ViewRot);
    else if (MasterAIController)
    {
        APawn* Pawn = MasterAIController->GetPawn();
        if (!Pawn) return false;
        ViewPoint = Pawn->GetActorLocation();
        ViewRot = Pawn->GetActorRotation();
    }
    else return false;

    // 트레이스 시작, 끝 계산
    FVector Start = ViewPoint;
    FVector End = Start + ViewRot.Vector() * MaxTargetDistance;
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 2.0f);

    // 라인트레이스 수행
    return GetWorld()->LineTraceSingleByChannel(OutHit, ViewPoint, ViewPoint + ViewRot.Vector() * MaxTargetDistance, ECC_Visibility);
}

// 타겟 확정 후 TargetData 생성 및 전송
void ATargetActorGA_SingleLineTrace::ConfirmTargetingAndContinue()
{
    FHitResult Hit;
    FGameplayTag GameTag;
    if (LineTraceFN(Hit))
    {
        HandleEffectProjectile(Hit, GameTag);
        FGameplayAbilityTargetDataHandle TargetData;
        FGameplayAbilityTargetData_SingleTargetHit* HitData = new FGameplayAbilityTargetData_SingleTargetHit(Hit);
        TargetDataReadyDelegate.Broadcast(TargetData);
    }
    else
    {
        TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
    }
    DestroyReticleActors();
}
