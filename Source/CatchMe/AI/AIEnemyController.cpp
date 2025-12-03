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

    UE_LOG(LogTemp, Log, TEXT("[AI] OnPossess %s, World=%s, NetMode=%d, Role=%d"),
        *InPawn->GetName(),
        *GetWorld()->GetName(),
        (int32)GetWorld()->GetNetMode(),
        (int32)GetLocalRole());

    if (BehaviorTreeAsset && HasAuthority())
    {
        UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp);
        RunBehaviorTree(BehaviorTreeAsset);

        if (InPawn && BlackboardComp)
        {
            BlackboardComp->SetValueAsVector(TEXT("HomeLocation"), InPawn->GetActorLocation());
        }
    }


}

void AAIEnemyController::SetTargetActor(AActor* Target)
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsObject("TargetActor", Target);
    }
}

void AAIEnemyController::SendDeadToBT()
{
    BlackboardComp->SetValueAsBool(TEXT("IsDead"), true);
}
