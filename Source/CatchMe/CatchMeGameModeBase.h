// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CatchMePlayerState.h"
#include "CatchMeGameModeBase.generated.h"




class ACatchMePlayerController;
class ASpawnVolume;
class ACatchMePolice;
class ACatchMeThief;

UENUM(BlueprintType)
enum class ETimerType : uint8
{
	PoliceSlow UMETA(DisplayName = "PoliceSlow"),
	ThiefSlow UMETA(DisplayName = "ThiefSlow"),
	Timer UMETA(DisplayName = "Timer"),
	GameOver UMETA(DisplayName = "GameOver"),
	None UMETA(DisplayName = "None"),
};

UCLASS()
class CATCHME_API ACatchMeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
	void StartGame();

	void ResetGame();

	void ResetGameState();

	void GameOver();

	void SpawnAndPossessPawnFor(ACatchMePlayerController* CMPC, EPlayerClass playerclass);

	void StartTimer();

	void ResetTimer();

	void EndTimer(ETimerType Type);

	void HandlePoliceWin();

	void HandleThiefWin();

	void KillCitizen();

	void InitSpawnVolumes();

	UFUNCTION(BlueprintCallable)
	void SpeedDownThief();

	UFUNCTION(BlueprintCallable)
	void SpeedDownPolice();

	UFUNCTION(BlueprintCallable)
	void HealPolice();


protected:

	TArray<TObjectPtr<ACatchMePlayerController>> AllPlayerControllers;

	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
	TSubclassOf<APawn> PolicePawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
	TSubclassOf<APawn> ThiefPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Test")
	TSubclassOf<AActor> TestItemClass1;

	UPROPERTY(EditDefaultsOnly, Category = "Test")
	TSubclassOf<AActor> TestItemClass2;

	UPROPERTY(EditDefaultsOnly, Category = "SpawnVolume")
	TSubclassOf<ASpawnVolume> SpawnVolume;

	int32 TimerCount;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxTimerCount;

	FTimerHandle TimerHandle;

	FTimerHandle GameOverTimerHandle;
	int32 GameOverTimerCount = 10;

	FTimerHandle ThiefSlowTimerHandle;
	int32 ThiefSlowTimerCount;

	FTimerHandle PoliceSlowTimerHandle;
	int32 PoliceSlowTimerCount;

	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
	ACatchMePolice* Police = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
	ACatchMeThief* Thief = nullptr;

public:
	UPROPERTY()
	ASpawnVolume* InWorldSpawnVolume = nullptr;
	
};


