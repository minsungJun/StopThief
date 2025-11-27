// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CatchMePlayerState.h"
#include "CatchMeGameModeBase.generated.h"




class ACatchMePlayerController;
//class ACatchMeThief;


UCLASS()
class CATCHME_API ACatchMeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	void StartGame();

	void SpawnAndPossessPawnFor(ACatchMePlayerController* CMPC, EPlayerClass playerclass);


	void StartTimer();

	void ResetTimer();

	void EndTimer();

	void HandlePoliceWin();

	void HandleThiefWin();

	void KillCitizen();


protected:

	TArray<TObjectPtr<ACatchMePlayerController>> AllPlayerControllers;

	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
	TSubclassOf<APawn> PolicePawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
	TSubclassOf<APawn> ThiefPawnClass;

	int32 TimerCount;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxTimerCount;

	FTimerHandle TimerHandle;
	
};


