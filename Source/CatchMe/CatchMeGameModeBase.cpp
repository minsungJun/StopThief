// Fill out your copyright notice in the Description page of Project Settings.

#include "CatchMeGameModeBase.h"
#include "CatchMePlayerController.h"
#include "CatchMeGameStateBase.h"


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
	// 나중에 랜덤으로 경찰 도둑 결정
	ACatchMePlayerState* CMPS1 = AllPlayerControllers[0]->GetPlayerState<ACatchMePlayerState>();
	CMPS1->PlayerClass = EPlayerClass::Police;
	

	ACatchMePlayerState* CMPS2 = AllPlayerControllers[1]->GetPlayerState<ACatchMePlayerState>();
	CMPS2->PlayerClass = EPlayerClass::Thief;


	// Class Logging
	const UEnum* EnumPtr = StaticEnum<EPlayerClass>();
	FText DisplayText = EnumPtr->GetDisplayNameTextByValue((int64)CMPS1->PlayerClass);
	UE_LOG(LogTemp, Log, TEXT("PlayerClass : %s"), *DisplayText.ToString());
    SpawnAndPossessPawnFor(AllPlayerControllers[0], CMPS1->PlayerClass);
    CMPS1->GetPawn()->AddActorLocalOffset(FVector(0.f, -100.f, 0.f));

	DisplayText = EnumPtr->GetDisplayNameTextByValue((int64)CMPS2->PlayerClass);
	UE_LOG(LogTemp, Log, TEXT("PlayerClass : %s"), *DisplayText.ToString());
    SpawnAndPossessPawnFor(AllPlayerControllers[1], CMPS2->PlayerClass);
    CMPS2->GetPawn()->AddActorLocalOffset(FVector(0.f, 100.f, 0.f));


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
