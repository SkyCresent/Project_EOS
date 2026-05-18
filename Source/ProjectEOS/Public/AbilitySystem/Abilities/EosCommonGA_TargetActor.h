// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "EosCommonGA_TargetActor.generated.h"

class UEosBaseCombatComponent;
class UEosPlayerCombatComponent;
class AGameplayAbility;
class AActor;
class AEosBaseProjectile;
/**
 * 
 */
UCLASS()
class PROJECTEOS_API AEosCommonGA_TargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
    AEosCommonGA_TargetActor();

    virtual void StartTargeting(UGameplayAbility* Ability) override;
    virtual void CancelTargeting() override;

protected:
    virtual void Tick(float DeltaSeconds) override;
    virtual void BeginPlay() override;

    virtual bool LineTraceFN(FHitResult& OutHit);
    virtual void ConfirmTargetingAndContinue() override;

    void HandleEffectProjectile(const FHitResult& Hit, const FGameplayTag& EventTag);
    AGameplayAbilityWorldReticle* SpawnReticleActor(FVector Location, FRotator Rotation);
    void DestroyReticleActors();
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    float MaxTargetDistance = 2000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    TSubclassOf<AGameplayAbilityWorldReticle> CustomReticleClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
    FWorldReticleParameters CustomReticleParams;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    FName ProjectileSpawnSocketName = "Muzzle";
    UPROPERTY()
    AGameplayAbilityWorldReticle* MyReticleActor = nullptr;

    UPROPERTY()
    APlayerController* EosPrimaryPC = nullptr;

    UPROPERTY()
    AAIController* MasterAIController = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Socket")
    FName RightHandSocketName = "RightHandSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Socket")
    FName LeftHandSocketName = "LeftHandSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Socket")
    FName TwoHandSocketName = "TwoHandSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Socket")
    bool bUseLeftHandSocket = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Socket")
    bool bUseTwoHandSocket = false;
public:
    UFUNCTION(BlueprintPure, Category = "Targeting")
    FVector GetReticleLocation() const { return MyReticleActor ? MyReticleActor->GetActorLocation() : FVector::ZeroVector; }

    UFUNCTION(BlueprintPure, Category = "Targeting")
    FRotator GetReticleRotation() const { return MyReticleActor ? MyReticleActor->GetActorRotation() : FRotator::ZeroRotator; }
};

