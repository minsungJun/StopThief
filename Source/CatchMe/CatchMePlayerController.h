// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CatchMePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CATCHME_API ACatchMePlayerController : public APlayerController
{
	GENERATED_BODY()



public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	
protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> TimerTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> TimerTextWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PoliceHPTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> PoliceHPTextWidgetInstance;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CrossHairWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> CrossHairWidgetInstance;

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> StartButtonWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> StartButtonWidgetInstance;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FText TimerText;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FText PoliceHPText;


	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_CallStartGame();

	UFUNCTION(Client, Reliable)
	void Client_HideStartButton();

	UFUNCTION(Client, Reliable)
	void Client_ShowStartButton();



};
