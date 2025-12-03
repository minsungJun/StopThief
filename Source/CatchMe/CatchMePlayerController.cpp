// Fill out your copyright notice in the Description page of Project Settings.


#include "CatchMePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "CatchMeGameModeBase.h"

void ACatchMePlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TimerText);
	DOREPLIFETIME(ThisClass, NotificationText);
	DOREPLIFETIME(ThisClass, PoliceHPText);

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

	if (IsValid(PoliceHPTextWidgetClass) == true)
	{
		PoliceHPTextWidgetInstance = CreateWidget<UUserWidget>(this, PoliceHPTextWidgetClass);
		if (IsValid(PoliceHPTextWidgetInstance) == true)
		{
			PoliceHPTextWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(CrossHairWidgetClass) == true)
	{
		CrossHairWidgetInstance = CreateWidget<UUserWidget>(this, CrossHairWidgetClass);
		if (IsValid(CrossHairWidgetInstance) == true)
		{
			CrossHairWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(StartButtonWidgetClass) == true)
	{
		StartButtonWidgetInstance = CreateWidget<UUserWidget>(this, StartButtonWidgetClass);
		if (IsValid(StartButtonWidgetInstance) == true)
		{
			StartButtonWidgetInstance->AddToViewport();
		}
	}
}


void ACatchMePlayerController::Server_CallStartGame_Implementation()
{
	if (ACatchMeGameModeBase* GM = GetWorld()->GetAuthGameMode<ACatchMeGameModeBase>())
	{
		GM->StartGame();
	}
}

void ACatchMePlayerController::Client_HideStartButton_Implementation()
{
	if (IsValid(StartButtonWidgetInstance))
	{
		StartButtonWidgetInstance->RemoveFromParent();
		StartButtonWidgetInstance = nullptr;
	}
}

void ACatchMePlayerController::Client_ShowStartButton_Implementation()
{
	if (IsValid(StartButtonWidgetInstance))
		return;

	else
	{
		StartButtonWidgetInstance = CreateWidget<UUserWidget>(this, StartButtonWidgetClass);
		if (IsValid(StartButtonWidgetInstance) == true)
		{
			StartButtonWidgetInstance->AddToViewport();
		}
	}

}
