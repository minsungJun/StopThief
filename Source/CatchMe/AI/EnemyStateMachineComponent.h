#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyState.h"
#include "EnemyStateMachineComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CATCHME_API UEnemyStateMachineComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEnemyStateMachineComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // 현재 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
    EEnemyState CurrentState;

    // 상태 전환 함수
    UFUNCTION(BlueprintCallable, Category = "FSM")
    void ChangeState(EEnemyState NewState);

protected:
    // 상태 진입 / 종료 / 유지 처리
    virtual void OnEnterState(EEnemyState State);
    virtual void OnExitState(EEnemyState State);
    virtual void TickState(EEnemyState State, float DeltaTime);
};
