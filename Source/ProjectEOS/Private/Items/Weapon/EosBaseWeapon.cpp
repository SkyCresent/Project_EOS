// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon/EosBaseWeapon.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystem.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEosBaseWeapon::AEosBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	SubWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SubWeaponMesh"));
	
	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionBox"));
	WeaponCollisionBox->SetupAttachment(WeaponMesh);
	WeaponCollisionBox->SetBoxExtent(FVector(20.f));
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponCollisionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Overlap);
	WeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	WeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);

	SubWeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SubWeaponCollisionBox"));
	SubWeaponCollisionBox->SetupAttachment(SubWeaponMesh);
	SubWeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SubWeaponCollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	SubWeaponCollisionBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	SubWeaponCollisionBox->SetBoxExtent(FVector(20.f));
	SubWeaponCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxBeginOverlap);
	SubWeaponCollisionBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnCollisionBoxEndOverlap);

	ParticleWeaponR = CreateDefaultSubobject<UParticleSystem>(TEXT("ParticleWeaponR"));
	ParticleWeaponL = CreateDefaultSubobject<UParticleSystem>(TEXT("ParticleWeaponL"));
}

// Called when the game starts or when spawned
void AEosBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SubWeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEosBaseWeapon::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("forgot to assign an instigator as the owning pawn for the weapon: %s"), *GetName());
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (WeaponOwningPawn != HitPawn)
		{
			OnWepaonHitTarget.ExecuteIfBound(OtherActor);
		}
	}
}

void AEosBaseWeapon::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	checkf(WeaponOwningPawn, TEXT("forgot to assign an instigator as the owning pawn for the weapon: %s"), *GetName());
	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if (WeaponOwningPawn != HitPawn)
		{
			OnWeaponEndHitFromTarget.ExecuteIfBound(OtherActor);
		}
	}
}

// Called every frame
void AEosBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}