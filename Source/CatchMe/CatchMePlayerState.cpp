// Fill out your copyright notice in the Description page of Project Settings.


#include "CatchMePlayerState.h"

#include "Net/UnrealNetwork.h"

ACatchMePlayerState::ACatchMePlayerState()
{
	PlayerNameString = TEXT("TestName");
	PlayerClass = EPlayerClass::None;
	bReplicates = true;
}

void ACatchMePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, PlayerClass);

}
