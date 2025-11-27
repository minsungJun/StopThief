#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIEnemyController.generated.h"

UCLASS()
class CATCHME_API AAIEnemyController : public AAIController
{
    GENERATED_BODY()
    AAIEnemyController();

public:
    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION(BlueprintCallable)
    void SetTargetActor(AActor* Target);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UBlackboardComponent* BlackboardComp;

}; 
