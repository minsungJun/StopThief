// Copyright Epic Games, Inc. All Rights Reserved.

#include "CatchMeGameMode.h"
#include "CatchMeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACatchMeGameMode::ACatchMeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
