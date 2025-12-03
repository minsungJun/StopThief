// Fill out your copyright notice in the Description page of Project Settings.

#include "CatchMeGameModeBase.h"
#include "CatchMePlayerController.h"
#include "CatchMeGameStateBase.h"
#include "CatchMeItem.h"
#include "SpawnVolume.h"
#include "AI/AIEnemyCharacter.h"
#include "CatchMePolice.h"
#include "CatchMeThief.h"
#include "Blueprint/UserWidget.h"


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

		//
		//{
		//	StartGame();
		//}
	}
}

void ACatchMeGameModeBase::StartGame()
{
	// 배열 안에 있는 플레이어에게 역할을 부여
	// PlayerState에 PlayerClass변경
	// 나중에 랜덤으로 경찰 도둑 결정하도록 수정
	// 
	// 게임 종료시에 볼륨 초기화 하든 삭제하고 새로 생성하든 처리
	if (AllPlayerControllers.Num() < 2)
		return;

	// GameState 초기화
	ResetGameState();
	
	InitSpawnVolumes();

	// AI소환
	InWorldSpawnVolume->SpawnAI();


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

	//CMPS1->GetPawn()->AddActorLocalOffset(FVector(0.f, -100.f, 0.f));
    //CMPS2->GetPawn()->AddActorLocalOffset(FVector(0.f, 100.f, 0.f));



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
		CMPC->Client_HideStartButton();
	}


	StartTimer();

}

void ACatchMeGameModeBase::ResetGame()
{
	if (!HasAuthority())
		return;
	UE_LOG(LogTemp, Warning, TEXT("ResetGame Start"));
	// 플레이어 캐릭터 제거 후 기본 캐릭터로 변경
	ACatchMePlayerState* CMPS1 = AllPlayerControllers[0]->GetPlayerState<ACatchMePlayerState>();
	CMPS1->PlayerClass = EPlayerClass::None;

	ACatchMePlayerState* CMPS2 = AllPlayerControllers[1]->GetPlayerState<ACatchMePlayerState>();
	CMPS2->PlayerClass = EPlayerClass::None;

	SpawnAndPossessPawnFor(AllPlayerControllers[0], CMPS1->PlayerClass);
	SpawnAndPossessPawnFor(AllPlayerControllers[1], CMPS2->PlayerClass);

	// 필드에 있는 AI / 아이템 파괴
	for (int32 i = InWorldSpawnVolume->AIArray.Num() - 1; i >= 0; --i)
	{
		AAIEnemyCharacter* Enemy = InWorldSpawnVolume->AIArray[i];

		if (!IsValid(Enemy))
		{
			InWorldSpawnVolume->AIArray.RemoveAt(i);
			continue;
		}

		// 여기서 Destroy() 호출
		Enemy->Destroy();

		// Destroy될 애는 배열에서도 제거
		InWorldSpawnVolume->AIArray.RemoveAt(i);
	}

	for (auto& Item : InWorldSpawnVolume->ItemArray)
	{
		Item->Destroy();
	}

	for (int32 i = InWorldSpawnVolume->ItemArray.Num() - 1; i >= 0; --i)
	{
		ACatchMeItem* Item = InWorldSpawnVolume->ItemArray[i];

		if (!IsValid(Item))
		{
			InWorldSpawnVolume->ItemArray.RemoveAt(i);
			continue;
		}

		// 여기서 Destroy() 호출
		Item->Destroy();

		// Destroy될 애는 배열에서도 제거
		InWorldSpawnVolume->ItemArray.RemoveAt(i);
	}

	InWorldSpawnVolume->AIArray.Empty();

	InWorldSpawnVolume->ItemArray.Empty();

	// UI 초기화
	for (const auto& CMPC : AllPlayerControllers)
	{
		FString CombinedMessageString = TEXT("");
		CMPC->TimerText = FText::FromString(CombinedMessageString);
		CMPC->NotificationText = FText::FromString(CombinedMessageString);
		CMPC->PoliceHPText = FText::FromString(CombinedMessageString);

		CMPC->Client_ShowStartButton();
	}

}

void ACatchMeGameModeBase::ResetGameState()
{
	ACatchMeGameStateBase* GS = GetGameState<ACatchMeGameStateBase>();
	if (!GS) return;
	GS->PoliceHP = 5;
}

void ACatchMeGameModeBase::GameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("GameOverTiemr Start"));

	GameOverTimerCount = 10;
	GetWorld()->GetTimerManager().SetTimer(
		GameOverTimerHandle, 
		FTimerDelegate::CreateLambda([&]
			{
				--GameOverTimerCount;
				EndTimer(ETimerType::GameOver);
			}
		),
		1.0f, 
		true, 
		0.0f
	);
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
		PawnClassToSpawn = PolicePawnClass;
        UE_LOG(LogTemp, Log, TEXT("default"));
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

    UWorld* World = GetWorld();
    if (!World) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = CMPC;
    SpawnParams.Instigator = nullptr;


	APawn* NewPawn;
	if (InWorldSpawnVolume)
	{
		FVector Location;
		for (int32 Try = 0; Try < 10; ++Try)
		{
			Location = InWorldSpawnVolume->GetRandomPointInVolume();
			if (InWorldSpawnVolume->CanSpawnAtLocation(Location, 1.f))
				break;
			if (Try == 9)
				Location = FVector::ZeroVector;
		}

		FTransform NewTransform = FTransform(FRotator::ZeroRotator, Location);
		switch (playerclass)
		{
		case EPlayerClass::Police:
			Police = World->SpawnActor<ACatchMePolice>(PawnClassToSpawn, NewTransform, SpawnParams);
			CMPC->Possess(Police);
			UE_LOG(LogTemp, Log, TEXT("EPlayerClass::Police Possess"));
			break;
		case EPlayerClass::Thief:
			Thief = World->SpawnActor<ACatchMeThief>(PawnClassToSpawn, NewTransform, SpawnParams);
			CMPC->Possess(Thief);
			UE_LOG(LogTemp, Log, TEXT("EPlayerClass::Thief Possess"));
			break;
		default:
			NewPawn = World->SpawnActor<APawn>(PawnClassToSpawn, NewTransform, SpawnParams);
			CMPC->Possess(NewPawn);
			UE_LOG(LogTemp, Log, TEXT("NewPawn Possess"));
			break;
		}
		UE_LOG(LogTemp, Warning, TEXT("GetRandomPointInVolume"));

	}
	else
	{
		NewPawn = World->SpawnActor<APawn>(PawnClassToSpawn, SpawnTransform, SpawnParams);
		CMPC->Possess(NewPawn);
	}

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
				--TimerCount;
				for (const auto& CMPC : AllPlayerControllers)
				{
					FString CombinedMessageString = TEXT("Time Left : ") + FString::FromInt(TimerCount);
					CMPC->TimerText = FText::FromString(CombinedMessageString);
				}
				EndTimer(ETimerType::Timer);
			}
		), 1.0f, true, 0.0f);

}

void ACatchMeGameModeBase::ResetTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ACatchMeGameModeBase::EndTimer(ETimerType Type)
{
	switch (Type)
	{
		case ETimerType::Timer:
			if (TimerCount <= 0)
			{
				HandleThiefWin();
				ResetTimer();
			}
			else if (TimerCount % 15 <= 0)
			{
				InWorldSpawnVolume->SpawnItem();
			}
			else if (TimerCount == 30)
			{
				InWorldSpawnVolume->SpawnHealItem();
				InWorldSpawnVolume->SpawnHealItem();
			}
			break;

		case ETimerType::PoliceSlow:
			if (PoliceSlowTimerCount <= 0)
			{
				Police->ChangingSpeed = 0;
				Police->UpdateMovementSpeed();

				GetWorld()->GetTimerManager().ClearTimer(PoliceSlowTimerHandle);
			}
			
			break;

		case ETimerType::ThiefSlow:
			if (ThiefSlowTimerCount <= 0)
			{
				if (InWorldSpawnVolume)
				{
					for (auto& Enemy : InWorldSpawnVolume->AIArray)
					{
						if (!IsValid(Enemy))
						{
							continue;
						}
						Enemy->ChangingSpeed = 0;
						Enemy->UpdateMovementSpeed();
					}
				}
				Thief->ChangingSpeed = 0;
				Thief->UpdateMovementSpeed();

				GetWorld()->GetTimerManager().ClearTimer(ThiefSlowTimerHandle);
			}
			break;

		case ETimerType::GameOver:
			if (GameOverTimerCount <= 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("EndTimer Start"));
				ResetGame();

				GetWorld()->GetTimerManager().ClearTimer(GameOverTimerHandle);
			}
			break;

		case ETimerType::None:
			break;
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

	GameOver();
}

void ACatchMeGameModeBase::HandleThiefWin()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleThiefWin"));
	for (const auto& CMPC : AllPlayerControllers)
	{
		FString CombinedMessageString = TEXT("Thief Win");
		CMPC->NotificationText = FText::FromString(CombinedMessageString);
	}

	GameOver();
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

void ACatchMeGameModeBase::InitSpawnVolumes()
{
	if (InWorldSpawnVolume == nullptr)
	{
		InWorldSpawnVolume = GetWorld()->SpawnActor<ASpawnVolume>(
			SpawnVolume,
			FVector(0, 0, -20),
			FRotator::ZeroRotator
		);
	}

}

void ACatchMeGameModeBase::SpeedDownThief()
{
	if (InWorldSpawnVolume == nullptr)
		return;

	if (Thief == nullptr)
		return;

	for (auto& Enemy : InWorldSpawnVolume->AIArray)
	{
		if (!IsValid(Enemy))
		{
			continue;
		}
		Enemy->ChangingSpeed = -100;
		Enemy->UpdateMovementSpeed();
	}

	Thief->ChangingSpeed = -100;
	Thief->UpdateMovementSpeed();

	ThiefSlowTimerCount = 5;

	GetWorld()->GetTimerManager().SetTimer(
		ThiefSlowTimerHandle, FTimerDelegate::CreateLambda([&]
			{
				--ThiefSlowTimerCount;
				EndTimer(ETimerType::ThiefSlow);

			}
		), 1.0f, true, 0.0f);


}

void ACatchMeGameModeBase::SpeedDownPolice()
{
	if (Police == nullptr)
		return;

	Police->ChangingSpeed = -100;
	Police->UpdateMovementSpeed();

	PoliceSlowTimerCount = 5;

	GetWorld()->GetTimerManager().SetTimer(
		PoliceSlowTimerHandle, FTimerDelegate::CreateLambda([&]
			{
				--PoliceSlowTimerCount;
				EndTimer(ETimerType::PoliceSlow);

			}
		), 1.0f, true, 0.0f);
}

void ACatchMeGameModeBase::HealPolice()
{
	if (Police == nullptr)
		return;

	ACatchMeGameStateBase* GS = GetGameState<ACatchMeGameStateBase>();
	if (!GS) return;

	GS->PoliceHP++;

	for (const auto& CMPC : AllPlayerControllers)
	{
		ACatchMePlayerState* CMPS = CMPC->GetPlayerState<ACatchMePlayerState>();
		if (CMPS->PlayerClass == EPlayerClass::Police)
		{
			FString CombinedMessageString = TEXT("HP : ") + FString::FromInt(GS->PoliceHP);
			CMPC->PoliceHPText = FText::FromString(CombinedMessageString);
		}
	}

}
