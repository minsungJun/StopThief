#include "BTTask_FindPatrolPoint.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPatrolPoint::UBTTask_FindPatrolPoint()
{
    NodeName = TEXT("Find Patrol Point");
}

// BTTask_FindPatrolPoint.cpp
EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*)
{
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return EBTNodeResult::Failed;

    const FVector Home = BB->GetValueAsVector(HomeLocationKey.SelectedKeyName);

    float Radius = DefaultRadius;
    if (PatrolRadiusKey.SelectedKeyType)  // 키가 세팅돼 있으면
        Radius = BB->GetValueAsFloat(PatrolRadiusKey.SelectedKeyName);

    if (UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(OwnerComp.GetWorld()))
    {
        FNavLocation Out;
        if (Nav->GetRandomReachablePointInRadius(Home, Radius, Out))
        {
            BB->SetValueAsVector(MoveToLocationKey.SelectedKeyName, Out.Location);
            return EBTNodeResult::Succeeded;
        }
    }
    return EBTNodeResult::Failed;
}

