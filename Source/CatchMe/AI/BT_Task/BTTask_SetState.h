#pragma once
//JMS
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AI/EnemyState.h"                          
#include "BTTask_SetState.generated.h"

UCLASS()
class CATCHME_API UBTTask_SetState : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_SetState();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    EEnemyState TargetState = EEnemyState::Idle;

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
