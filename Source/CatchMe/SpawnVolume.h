// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;
class AAIEnemyCharacter;
class ACatchMeItem;

UCLASS()
class CATCHME_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	// 스폰 영역을 담당할 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	FVector GetRandomPointInVolume() const;

protected:


	UPROPERTY(EditDefaultsOnly, Category = "Pawn")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<AActor> ThiefItemClass_1;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<AActor> PoliceItemClass_1;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<AActor> PoliceItemClass_2;



public:	

	void SpawnAll();
	void SpawnAI();
	void SpawnItem();
	void SpawnHealItem();

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TArray<TObjectPtr<AAIEnemyCharacter>> AIArray;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	TArray<TObjectPtr<ACatchMeItem>> ItemArray;

	bool CanSpawnAtLocation(const FVector& Location, float Radius);

};
