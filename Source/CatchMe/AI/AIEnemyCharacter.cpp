#include "AIEnemyCharacter.h"
#include "AIEnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyStateMachineComponent.h"
#include "../CatchMeGameModeBase.h"
#include "Net/UnrealNetwork.h"

AAIEnemyCharacter::AAIEnemyCharacter()
{

}

void AAIEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (!StateMachine)
        StateMachine = FindComponentByClass<UEnemyStateMachineComponent>();

    //Controller를 얻어왔는지 확인
    if (HasAuthority())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("[SERVER] Enemy BeginPlay, Controller = %s"),
            *GetNameSafe(GetController()));
    }
    else
    {
        UE_LOG(LogTemp, Warning,
            TEXT("[CLIENT] Enemy BeginPlay, Controller = %s"),
            *GetNameSafe(GetController()));
    }

}

void AAIEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, CurrentHP);
}


float AAIEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (!HasAuthority() || bIsDead || DamageAmount <= 0.f)
    {
        return 0.f;
    }

    CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.f, MaxHP);

    UE_LOG(LogTemp, Log, TEXT("[Thief] Took damage: %.1f, HP: %.1f"), DamageAmount, CurrentHP);

    if (CurrentHP <= 0.f)
    {
        bIsDead = true;
        Die(EventInstigator);
    }

    return DamageAmount;
}


void AAIEnemyCharacter::Die(AController* KillerController)
{
    if (!HasAuthority())
        return;

    UE_LOG(LogTemp, Log, TEXT("[Citizen] Dead"));

    if (UWorld* World = GetWorld())
    {
        if (ACatchMeGameModeBase* GM = World->GetAuthGameMode<ACatchMeGameModeBase>())
        {
            GM->KillCitizen();

        }
    }

    // 나중에 래그돌/리스폰 넣고 싶으면 Destroy 대신 다른 처리
    Destroy();
}

