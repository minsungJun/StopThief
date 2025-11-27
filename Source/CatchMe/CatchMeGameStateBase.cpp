#include "CatchMeGameStateBase.h"
#include "Net/UnrealNetwork.h"

void ACatchMeGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, PoliceHP);

}