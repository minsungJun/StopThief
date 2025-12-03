// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CatchMeGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class CATCHME_API ACatchMeGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UPROPERTY(Replicated)
	int32 PoliceHP = 5;



};
