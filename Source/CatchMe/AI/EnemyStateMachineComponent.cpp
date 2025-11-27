#include "EnemyStateMachineComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

UEnemyStateMachineComponent::UEnemyStateMachineComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    CurrentState = EEnemyState::Patrol;
}

void UEnemyStateMachineComponent::BeginPlay()
{
    Super::BeginPlay();
    OnEnterState(CurrentState); // 시작 상태 진입
}

void UEnemyStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    TickState(CurrentState, DeltaTime);
}

void UEnemyStateMachineComponent::ChangeState(EEnemyState NewState)
{
    if (NewState == CurrentState)
        return;

    OnExitState(CurrentState);
    CurrentState = NewState;
    OnEnterState(NewState);

    UE_LOG(LogTemp, Log, TEXT("[%s] changed to state: %s"),
        *GetOwner()->GetName(),
        *UEnum::GetValueAsString(NewState));
}

void UEnemyStateMachineComponent::OnEnterState(EEnemyState State)
{
    switch (State)
    {
    case EEnemyState::Idle:
        UE_LOG(LogTemp, Log, TEXT("Entered Idle"));
        break;

    case EEnemyState::Patrol:
        UE_LOG(LogTemp, Log, TEXT("Entered Patrol"));
        break;

    case EEnemyState::Dead:
        UE_LOG(LogTemp, Log, TEXT("Entered Dead"));
        break;
    }
}

void UEnemyStateMachineComponent::OnExitState(EEnemyState State)
{
    switch (State)
    {
    case EEnemyState::Idle:
        UE_LOG(LogTemp, Log, TEXT("Exit Idle"));
        break;
    case EEnemyState::Attack:
        break;
    default:
        break;
    }
}

void UEnemyStateMachineComponent::TickState(EEnemyState State, float DeltaTime)
{
    switch (State)
    {
    case EEnemyState::Idle:
        // Idle 동안 지속 처리할 로직
        break;

    case EEnemyState::Chase:
        // Chase 동안 AI가 추적 중일 때 처리
        break;

    case EEnemyState::Attack:
        // 공격 상태 유지 중
        break;
    }
}
