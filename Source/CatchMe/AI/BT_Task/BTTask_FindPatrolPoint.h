#pragma once
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPoint.generated.h"

UCLASS()
class CATCHME_API UBTTask_FindPatrolPoint : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_FindPatrolPoint();

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector HomeLocationKey;    // Vector

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector MoveToLocationKey;  // Vector

    // ← 반경을 BB에서 읽는다
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector PatrolRadiusKey;    // Float

    // (안전용) BB에 없으면 쓸 기본값
    UPROPERTY(EditAnywhere, Category = "Patrol")
    float DefaultRadius = 600.f;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
