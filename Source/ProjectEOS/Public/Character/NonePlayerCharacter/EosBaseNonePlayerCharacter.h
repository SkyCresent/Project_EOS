// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/EosBaseCharacter.h"
#include "ProjectEOS/Public/Interfaces/EosInteractableInterface.h"
#include "EosBaseNonePlayerCharacter.generated.h"

class AEosBasePlayerController;

/**
 * 
 */
UCLASS()
class PROJECTEOS_API AEosBaseNonePlayerCharacter : public AEosBaseCharacter, public IEosInteractableInterface
{
	GENERATED_BODY()
	
public:
	AEosBaseNonePlayerCharacter();

	//아이템의 이름을 저장할 변수 블루프린트에서 가져와서 데이터테이블의 이름을 넣어준다
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Name;

protected:
	virtual void BeginPlay()override;

private:	

public:
	//상호작용 함수
	virtual void InteractWith(AActor* InterActor);

/// CUSUTOM FUNCTION
	//아이템의 이름을 보여줄 함수
	UFUNCTION(BlueprintCallable)
	virtual FText LookAt();

	UFUNCTION(BlueprintPure)
	AEosBasePlayerController* GetEosPlayerController(AActor* Actor) const;


/// CUSUTOM VARIABLE
private:
	AEosBasePlayerController* EosPlayerController;

	UPROPERTY(EditAnywhere)
	FName NpcID;

	UPROPERTY(EditAnywhere)
	FName DialogID;

public:

///  GETTER FUNCTION
public:
	FName GetDialogID() const;
};
