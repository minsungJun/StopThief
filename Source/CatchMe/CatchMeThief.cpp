// Fill out your copyright notice in the Description page of Project Settings.


#include "CatchMeThief.h"
#include "Net/UnrealNetwork.h"
#include "CatchMeGameModeBase.h"

ACatchMeThief::ACatchMeThief()
{
	bReplicates = true;
}

void ACatchMeThief::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        CurrentHP = MaxHP;
    }
}

float ACatchMeThief::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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



void ACatchMeThief::Die(AController* KillerController)
{
    if (!HasAuthority())
        return;

    UE_LOG(LogTemp, Log, TEXT("[Thief] Dead"));

    if (UWorld* World = GetWorld())
    {
        if (ACatchMeGameModeBase* GM = World->GetAuthGameMode<ACatchMeGameModeBase>())
        {
            GM->HandlePoliceWin();
        }
    }

    // 나중에 래그돌/리스폰 넣고 싶으면 Destroy 대신 다른 처리
    Destroy();
}

void ACatchMeThief::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACatchMeThief, CurrentHP);
}
