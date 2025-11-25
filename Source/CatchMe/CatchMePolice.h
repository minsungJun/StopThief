// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchMeCharacter.h"
#include "CatchMePolice.generated.h"

/**
 * 
 */
UCLASS()
class CATCHME_API ACatchMePolice : public ACatchMeCharacter
{
	GENERATED_BODY()
	

public:
	ACatchMePolice();

	void Fire();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Input")
	TObjectPtr<UInputAction> FireAction;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 서버에서 실제 히트스캔 처리
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& TraceEnd);

	void ServerFire_Implementation(const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& TraceEnd);

};

