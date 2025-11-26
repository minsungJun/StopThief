// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CatchMeCharacter.h"
#include "CatchMeThief.generated.h"

/**
 * 
 */
UCLASS()
class CATCHME_API ACatchMeThief : public ACatchMeCharacter
{
	GENERATED_BODY()
	
public:
	ACatchMeThief();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHP = 100.f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHP;

	virtual float TakeDamage(float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	void Die(AController* KillerController);

	bool bIsDead = false;
};
