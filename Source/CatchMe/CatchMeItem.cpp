#include "CatchMeItem.h"
#include "CatchMePlayerState.h"
#include "GameFramework/PlayerController.h"

ACatchMeItem::ACatchMeItem()
{
 
	bReplicates = true;

}

bool ACatchMeItem::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
    const APlayerController* PC = Cast<APlayerController>(RealViewer);
    const ACatchMePlayerState* PS = PC ? PC->GetPlayerState<ACatchMePlayerState>() : nullptr;
    const EPlayerClass PlayerClass = PS ? PS->PlayerClass : EPlayerClass::None;

    //UE_LOG(LogTemp, Warning, TEXT("[Item %s] Relevancy check: VisibleFor=%d, Viewer=%s, PlayerClass=%d"),
    //    *GetName(),
    //    (int32)VisibleFor,
    //    PC ? *PC->GetName() : TEXT("None"),
    //    (int32)PlayerClass
    //);

    // 기본 relevancy 먼저 체크
    if (!Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation))
    {
        return false;
    }

    if (VisibleFor == EItemVisibleFor::All)
    {
        return true;
    }

    if (!PC || !PS)
    {
        return false;
    }

    switch (VisibleFor)
    {
    case EItemVisibleFor::PoliceOnly:
        return PlayerClass == EPlayerClass::Police;

    case EItemVisibleFor::ThiefOnly:
        return PlayerClass == EPlayerClass::Thief;

    default:
        return true;
    }
}

