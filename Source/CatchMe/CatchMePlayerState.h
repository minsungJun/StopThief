// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CatchMePlayerState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerClass : uint8
{
	Police UMETA(DisplayName = "Police"),
	Thief UMETA(DisplayName = "Thief"),
	None UMETA(DisplayName = "None"),
};

UCLASS()
class CATCHME_API ACatchMePlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:
	ACatchMePlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;


public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	EPlayerClass PlayerClass;


};
