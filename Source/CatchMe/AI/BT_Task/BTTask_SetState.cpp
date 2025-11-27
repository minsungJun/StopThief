#include "BTTask_SetState.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "AI/EnemyStateMachineComponent.h"          // FSM 컴포넌트
#include "AI/AIEnemyCharacter.h"                    // 캐릭터 클래스(컴포넌트 찾기용)
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetState::UBTTask_SetState()
{
    NodeName = TEXT("Set Enemy State");
}

EBTNodeResult::Type UBTTask_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (!AICon) return EBTNodeResult::Failed;

    APawn* Pawn = AICon->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    if (UEnemyStateMachineComponent* FSM = Pawn->FindComponentByClass<UEnemyStateMachineComponent>())
    {
        FSM->ChangeState(TargetState);
    }
    else
    {
        return EBTNodeResult::Failed;
    }

    return EBTNodeResult::Succeeded;
}

