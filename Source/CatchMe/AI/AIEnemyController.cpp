#include "AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AAIEnemyController::AAIEnemyController()
{
    //BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
    //BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void AAIEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (BehaviorTreeAsset)
    {
        UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp);
        RunBehaviorTree(BehaviorTreeAsset);
    }

    if (InPawn && BlackboardComp)
    {
        BlackboardComp->SetValueAsVector(TEXT("HomeLocation"), InPawn->GetActorLocation());
        //InPawn->OnTakeAnyDamage.AddDynamic(this, &AAIEnemyController::OnTakeAnyDamage_Handled);
    }
}

void AAIEnemyController::SetTargetActor(AActor* Target)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsObject("TargetActor", Target);
    }
}
