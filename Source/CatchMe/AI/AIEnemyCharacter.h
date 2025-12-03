#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIEnemyCharacter.generated.h"

class UEnemyStateMachineComponent;

UCLASS()
class CATCHME_API AAIEnemyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAIEnemyCharacter();

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

    virtual float TakeDamage(float DamageAmount,
        FDamageEvent const& DamageEvent,
        AController* EventInstigator,
        AActor* DamageCauser) override;

    void Die(AController* KillerController);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastOnRagdoll();

    UFUNCTION()
    void UpdateMovementSpeed();

protected:
    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable)
    void ServerSetSprinting(bool bNewSprinting);

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UEnemyStateMachineComponent* StateMachine;


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
    float MaxHP = 100.f;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHP;

    bool bIsDead;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float WalkSpeed = 250;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float ChangingSpeed = 0;

};
