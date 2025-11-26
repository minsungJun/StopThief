// Fill out your copyright notice in the Description page of Project Settings.


#include "CatchMePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"

void ACatchMePlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TimerText);
	DOREPLIFETIME(ThisClass, NotificationText);

}

void ACatchMePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}


	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(TimerTextWidgetClass) == true)
	{
		TimerTextWidgetInstance = CreateWidget<UUserWidget>(this, TimerTextWidgetClass);
		if (IsValid(TimerTextWidgetInstance) == true)
		{
			TimerTextWidgetInstance->AddToViewport();
		}
	}
}
