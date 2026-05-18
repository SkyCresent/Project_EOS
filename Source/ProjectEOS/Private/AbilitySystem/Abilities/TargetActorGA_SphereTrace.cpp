// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TargetActorGA_SphereTrace.h"

// 타겟팅 시작 시 호출 : Ability와 컨트롤러 설정 + 리타클(Decal) 생성
void ATargetActorGA_SphereTrace::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);
    OwningAbility = Ability;
    MyReticleActor = SpawnReticleActor(GetActorLocation(), GetActorRotation());
    UE_LOG(LogTemp, Warning, TEXT("Reticle Spawn Attempt: %s"), *GetNameSafe(CustomReticleClass));
}

// 카메라 방향 기준으로 정면 -> 지면을 탐색하는 라인트레이스
bool ATargetActorGA_SphereTrace::LineTraceFN(FHitResult& OutHit)
{
    FVector ViewPoint;
    FRotator ViewRot;
    // Character(PrimaryPC) 또는 AI(MasterAIController)로부터 시점 얻기
    if (PrimaryPC) PrimaryPC->GetPlayerViewPoint(ViewPoint, ViewRot);
    else if (MasterAIController)
    {
        APawn* Pawn = MasterAIController->GetPawn();
        if (!Pawn) return false;
        ViewPoint = Pawn->GetActorLocation();
        ViewRot = Pawn->GetActorRotation();
    }
    else return false;

    //카메라 정면 방향으로 라인트레이스해서 가장 가까운 땅을 찍는다
    FVector TraceStart = ViewPoint;
    FVector TraceEnd = TraceStart + ViewRot.Vector() * MaxTargetDistance;;

    FHitResult LineHit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(SphereTraceForward), false);
    // 본인 캐릭터는 트레이스 무시
    if (APawn* IgnoredPawn = PrimaryPC ? PrimaryPC->GetPawn() : (MasterAIController ? MasterAIController->GetPawn() : nullptr))
    {
        Params.AddIgnoredActor(IgnoredPawn);
    }

    // 정면 방향으로 1차 라인트레이스
    if (GetWorld()->LineTraceSingleByChannel(LineHit, TraceStart, TraceEnd, ECC_Visibility, Params))
    {
        //첫 번째 충돌 지점 위로 300, 아래로 1000까지 2차 수직 트레이스
        FVector GroundStart = LineHit.ImpactPoint + FVector(0, 0, 300.f);
        FVector GroundEnd = LineHit.ImpactPoint - FVector(0, 0, 1000.f);

        FHitResult GroundHit;
        if (GetWorld()->LineTraceSingleByChannel(GroundHit, GroundStart, GroundEnd, ECC_Visibility, Params))
        {
            //최종 히트 지점 저장
            OutHit = GroundHit;

            DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 30.f, 12, FColor::Green, false, 2.0f);
            return true;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("SPHERE TRACE RUNNING"))
    return false;
}

// 타겟팅 확정 시 호출 : 트레이스 성공 여부에 따라 TargetData 생성 후 브로드캐스트
void ATargetActorGA_SphereTrace::ConfirmTargetingAndContinue()
{
    FHitResult Hit;
    if (LineTraceFN(Hit))
    {
        FGameplayAbilityTargetDataHandle Data;
        // Hit 데이터 생성
        Data.Add(new FGameplayAbilityTargetData_SingleTargetHit(Hit));
        // Hit 데이터 전송
        TargetDataReadyDelegate.Broadcast(Data);
    }
    else
    {
        TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
    }
    DestroyReticleActors();
}
