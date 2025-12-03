#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "AI/AIEnemyCharacter.h"
#include "CatchMeItem.h"


ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
}
	
FVector ASpawnVolume::GetRandomPointInVolume() const
{
    // 1) 박스 컴포넌트의 스케일된 Extent, 즉 x/y/z 방향으로 반지름(절반 길이)을 구함
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    // 2) 박스 중심 위치
    FVector BoxOrigin = SpawningBox->GetComponentLocation();

    // 3) 각 축별로 -Extent ~ +Extent 범위의 무작위 값 생성
    return BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
    );
}

void ASpawnVolume::SpawnAll()
{

}

void ASpawnVolume::SpawnAI()
{
    if (!AIPawnClass)
        return;

    if (HasAuthority())
    {
        int32 CountToSpawn = FMath::RandRange(10, 15);
        int32 MaxTriesPerSpawn = 10;

        for (int32 i = 0; i < CountToSpawn; ++i)
        {
            bool bSpawned = false;

            for (int32 Try = 0; Try < MaxTriesPerSpawn; ++Try)
            {
                FVector Location = GetRandomPointInVolume();

                if (CanSpawnAtLocation(Location, 1.f))
                {
                    AAIEnemyCharacter* Enemy = GetWorld()->SpawnActor<AAIEnemyCharacter>(
                        AIPawnClass,
                        Location,
                        FRotator::ZeroRotator
                    );

                    if (Enemy)
                    {
                        AIArray.Add(Enemy);
                        bSpawned = true;
                    }
                    break;
                }
            }

            if (!bSpawned)
            {
                UE_LOG(LogTemp, Warning, TEXT("[SpawnVolume] Failed to find free spot for AI %d"), i);
            }
        }
    }

}

void ASpawnVolume::SpawnItem()
{
    if (!ThiefItemClass_1 || !PoliceItemClass_1 || !PoliceItemClass_2)
        return;

    if (HasAuthority())
    {
        int32 MaxTriesPerSpawn = 10;

        for (int32 Try = 0; Try < MaxTriesPerSpawn; ++Try)
        {
            FVector Location = GetRandomPointInVolume();

            if (CanSpawnAtLocation(Location, 1.f))
            {
                ACatchMeItem* Item = GetWorld()->SpawnActor<ACatchMeItem>(
                    ThiefItemClass_1,
                    Location,
                    FRotator::ZeroRotator
                );

                if (Item)
                {
                    ItemArray.Add(Item);
                }
                break;
            }
        }

        for (int32 Try = 0; Try < MaxTriesPerSpawn; ++Try)
        {
            FVector Location = GetRandomPointInVolume();

            if (CanSpawnAtLocation(Location, 1.f))
            {
                ACatchMeItem* Item = GetWorld()->SpawnActor<ACatchMeItem>(
                    PoliceItemClass_1,
                    Location,
                    FRotator::ZeroRotator
                );

                if (Item)
                {
                    ItemArray.Add(Item);
                }
                break;
            }
        }

    }

}

void ASpawnVolume::SpawnHealItem()
{
    if (!ThiefItemClass_1 || !PoliceItemClass_1 || !PoliceItemClass_2)
        return;

    if (HasAuthority())
    {
        int32 MaxTriesPerSpawn = 10;

        for (int32 Try = 0; Try < MaxTriesPerSpawn; ++Try)
        {
            FVector Location = GetRandomPointInVolume();

            if (CanSpawnAtLocation(Location, 1.f))
            {
                ACatchMeItem* Item = GetWorld()->SpawnActor<ACatchMeItem>(
                    PoliceItemClass_2,
                    Location,
                    FRotator::ZeroRotator
                );

                if (Item)
                {
                    ItemArray.Add(Item);
                }
                break;
            }
        }
    }

}

bool ASpawnVolume::CanSpawnAtLocation(const FVector& Location, float Radius)
{
    UWorld* World = GetWorld();
    if (!World) return false;

    // 구 모양 충돌 범위
    FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

    FCollisionQueryParams Params(SCENE_QUERY_STAT(SpawnOverlap), false, this);

    // 해당 위치에 아무 액터나(지정 채널) 걸리는지 검사
    const bool bHit = World->OverlapAnyTestByChannel(
        Location,
        FQuat::Identity,
        ECC_Pawn,
        Sphere,
        Params
    );

    return !bHit;
}

