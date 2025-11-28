// Fill out your copyright notice in the Description page of Project Settings.

#include "CatchMeGameModeBase.h"
#include "CatchMePlayerController.h"
#include "CatchMeGameStateBase.h"
#include "CatchMeItem.h"


void ACatchMeGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	ACatchMePlayerController* CMPlayerController = Cast<ACatchMePlayerController>(NewPlayer);
	if (IsValid(CMPlayerController) == true)
	{
		//CMPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server.  \"Type\" /start to Start Game."));
		//이후에 필요하다면 추가

		AllPlayerControllers.Add(CMPlayerController);

		ACatchMePlayerState* CMPS = CMPlayerController->GetPlayerState<ACatchMePlayerState>();
		if (IsValid(CMPS) == true)
		{
			//플레이어 이름 설정
			CMPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ACatchMeGameStateBase* CMGameStateBase = GetGameState<ACatchMeGameStateBase>();
		if (IsValid(CMGameStateBase) == true)
		{
			//CMGameStateBase->MulticastRPCBroadcastLoginMessage(CMPS->PlayerNameString);
			//이후에 필요하다면 추가
		}

		if (AllPlayerControllers.Num() >= 2)
		{
			StartGame();
		}
	}
}

void ACatchMeGameModeBase::StartGame()
{
	// 배열 안에 있는 플레이어에게 역할을 부여
	// PlayerState에 PlayerClass변경
	// 나중에 랜덤으로 경찰 도둑 결정하도록 수정
	ACatchMePlayerState* CMPS1 = AllPlayerControllers[0]->GetPlayerState<ACatchMePlayerState>();
	CMPS1->PlayerClass = EPlayerClass::Police;
	

	ACatchMePlayerState* CMPS2 = AllPlayerControllers[1]->GetPlayerState<ACatchMePlayerState>();
	CMPS2->PlayerClass = EPlayerClass::Thief;


	// Class Logging
	const UEnum* EnumPtr = StaticEnum<EPlayerClass>();
	FText DisplayText = EnumPtr->GetDisplayNameTextByValue((int64)CMPS1->PlayerClass);
	UE_LOG(LogTemp, Log, TEXT("PlayerClass : %s"), *DisplayText.ToString());
    SpawnAndPossessPawnFor(AllPlayerControllers[0], CMPS1->PlayerClass);

	DisplayText = EnumPtr->GetDisplayNameTextByValue((int64)CMPS2->PlayerClass);
	UE_LOG(LogTemp, Log, TEXT("PlayerClass : %s"), *DisplayText.ToString());
    SpawnAndPossessPawnFor(AllPlayerControllers[1], CMPS2->PlayerClass);

	CMPS1->GetPawn()->AddActorLocalOffset(FVector(0.f, -100.f, 0.f));
    CMPS2->GetPawn()->AddActorLocalOffset(FVector(0.f, 100.f, 0.f));



	for (const auto& CMPC : AllPlayerControllers)
	{
		ACatchMePlayerState* CMPS = CMPC->GetPlayerState<ACatchMePlayerState>();
		if (CMPS->PlayerClass == EPlayerClass::Police)
		{
			ACatchMeGameStateBase* GS = GetGameState<ACatchMeGameStateBase>();
			if (!GS) return;
			FString CombinedMessageString = TEXT("HP : ") + FString::FromInt(GS->PoliceHP);
			CMPC->PoliceHPText = FText::FromString(CombinedMessageString);
		}
	}

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters Params;
		Params.Name = FName(TEXT("TestPolice")); // 필요하면 이름 지정 (선택)

		FVector Location = FVector(1300,1300,60);
		FRotator Rotation = FRotator::ZeroRotator;

		ACatchMeItem* NewItem = World->SpawnActor<ACatchMeItem>(
			TestItemClass1,   // TSubclassOf<ACatchMeItem>
			Location,
			Rotation,
			Params
		);
	}

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters Params;
		Params.Name = FName(TEXT("TestThief")); // 필요하면 이름 지정 (선택)

		FVector Location = FVector(1200, 1300, 60);
		FRotator Rotation = FRotator::ZeroRotator;

		ACatchMeItem* NewItem = World->SpawnActor<ACatchMeItem>(
			TestItemClass2,   // TSubclassOf<ACatchMeItem>
			Location,
			Rotation,
			Params
		);
	}

	StartTimer();

}




void ACatchMeGameModeBase::SpawnAndPossessPawnFor(ACatchMePlayerController* CMPC, EPlayerClass playerclass)
{
    if (!CMPC)
        return;

    TSubclassOf<APawn> PawnClassToSpawn = nullptr;

    const UEnum* EnumPtr = StaticEnum<EPlayerClass>();
    FText DisplayText = EnumPtr->GetDisplayNameTextByValue((int64)playerclass);
    UE_LOG(LogTemp, Log, TEXT("SpawnAndPossessPawnFor PlayerClass : %s"), *DisplayText.ToString());

    switch (playerclass)
    {
    case EPlayerClass::Police:
        PawnClassToSpawn = PolicePawnClass;
        UE_LOG(LogTemp, Log, TEXT("EPlayerClass::Police"));
        break;
    case EPlayerClass::Thief:
        PawnClassToSpawn = ThiefPawnClass;
        UE_LOG(LogTemp, Log, TEXT("EPlayerClass::Thief"));
        break;
    default:
        UE_LOG(LogTemp, Log, TEXT("EPlayerClass::Thief"));
        break;
    }

    if (!PawnClassToSpawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("PawnClassToSpawn is null for role %d"), (int32)playerclass);
        return;
    }

    // PlayerStart 위치 찾기 (기본 하나 쓰는 간단 버전)
    AActor* ExistingActor = CMPC->GetPawn();

    AActor* StartSpot = FindPlayerStart(CMPC);
    FTransform SpawnTransform = StartSpot
        ? StartSpot->GetActorTransform()
        : FTransform(FRotator::ZeroRotator, FVector(0.f, 0.f, 100.f));

    //if (ExistingActor)
    //{
    //    SpawnTransform = ExistingActor->GetActorTransform();
    //}
    //else
    //{
    //    SpawnTransform = FTransform::Identity;
    //}

    UWorld* World = GetWorld();
    if (!World) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = CMPC;
    SpawnParams.Instigator = nullptr;

    APawn* NewPawn = World->SpawnActor<APawn>(PawnClassToSpawn, SpawnTransform, SpawnParams);
    if (!NewPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn pawn for role %d"), (int32)playerclass);
        return;
    }
    CMPC->Possess(NewPawn);
    if (ExistingActor)
    {
        ExistingActor->Destroy();
    }

}


void ACatchMeGameModeBase::StartTimer()
{
	// 타이머 최대 시간을 BP에서 수정 가능하게 MaxTimerCount를 이용
	TimerCount = MaxTimerCount;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, FTimerDelegate::CreateLambda([&]
			{
				// 타이머가 시작되면 게임 중인 플레이어의 타이머 수정
				for (const auto& CMPC : AllPlayerControllers)
				{
					FString CombinedMessageString = TEXT("Time Left : ") + FString::FromInt(TimerCount);
					CMPC->TimerText = FText::FromString(CombinedMessageString);
				}
				--TimerCount;
				EndTimer();
			}
		), 1.0f, true, 0.0f);

}

void ACatchMeGameModeBase::ResetTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ACatchMeGameModeBase::EndTimer()
{
	if (TimerCount == 0)
	{
		HandleThiefWin();
		ResetTimer();
	}

}

void ACatchMeGameModeBase::HandlePoliceWin()
{
	ResetTimer();
	UE_LOG(LogTemp, Warning, TEXT("HandlePoliceWin"));
	for (const auto& CMPC : AllPlayerControllers)
	{
		FString CombinedMessageString = TEXT("Police Win");
		CMPC->NotificationText = FText::FromString(CombinedMessageString);
	}
}

void ACatchMeGameModeBase::HandleThiefWin()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleThiefWin"));
	for (const auto& CMPC : AllPlayerControllers)
	{
		FString CombinedMessageString = TEXT("Thief Win");
		CMPC->NotificationText = FText::FromString(CombinedMessageString);
	}
}

void ACatchMeGameModeBase::KillCitizen()
{
	ACatchMeGameStateBase* GS = GetGameState<ACatchMeGameStateBase>();
	if (!GS) return;
	GS->PoliceHP--;


	for (const auto& CMPC : AllPlayerControllers)
	{
		ACatchMePlayerState* CMPS = CMPC->GetPlayerState<ACatchMePlayerState>();
		if (CMPS->PlayerClass == EPlayerClass::Police)
		{
			FString CombinedMessageString = TEXT("HP : ") + FString::FromInt(GS->PoliceHP);
			CMPC->PoliceHPText = FText::FromString(CombinedMessageString);
		}
	}

	if (GS->PoliceHP <= 0)
		HandleThiefWin();
}
