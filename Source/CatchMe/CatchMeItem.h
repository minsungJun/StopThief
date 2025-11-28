// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CatchMeItem.generated.h"

UENUM(BlueprintType)
enum class EItemVisibleFor : uint8
{
	All,
	PoliceOnly,
	ThiefOnly,
};

UCLASS()
class CATCHME_API ACatchMeItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACatchMeItem();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EItemVisibleFor VisibleFor = EItemVisibleFor::All;

protected:
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
};
