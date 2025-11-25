// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CatchMePlayerState.h"
#include "CatchMeGameModeBase.generated.h"




class ACatchMePlayerController;


UCLASS()
class CATCHME_API ACatchMeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	void StartGame();

	void SpawnAndPossessPawnFor(ACatchMePlayerController* CMPC, EPlayerClass playerclass);


protected:

	TArray<TObjectPtr<ACatchMePlayerController>> AllPlayerControllers;

	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
	TSubclassOf<APawn> PolicePawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
	TSubclassOf<APawn> ThiefPawnClass;
	
};


