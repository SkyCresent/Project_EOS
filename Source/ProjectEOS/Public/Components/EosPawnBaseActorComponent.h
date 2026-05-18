// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EosPawnBaseActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTEOS_API UEosPawnBaseActorComponent : public UActorComponent
{
	GENERATED_BODY()

///ENGINE FUNCTION

public:	
	// Sets default values for this component's properties
	UEosPawnBaseActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

///CUSTOM FUNCTION
	
protected:
	template <class T>
	T* GetOwningPawn() const
	{
		//APawn 타입 변환 가능 점검
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' Template Parameter to GetPawn must be derived from APawn");
		
		//형변환
		return CastChecked<T>(GetOwner());
	}

	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}
	
	template <class T>
	T* GetOwningController() const
	{
		//AController 타입 변환 가능 점검
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' Template Parameter to GetController must be derived from AController");
		return GetOwningPawn<APawn>()->GetController<T>();
	}
};
