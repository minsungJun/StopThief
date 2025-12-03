// Fill out your copyright notice in the Description page of Project Settings.


#include "CatchMePolice.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "DrawDebugHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "Net/UnrealNetwork.h"

ACatchMePolice::ACatchMePolice()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACatchMePolice::Fire()
{
	if (!IsLocallyControlled())
		return;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
		return;

    const float Now = GetWorld()->GetTimeSeconds();
    if (Now < NextAllowedFireTime)
        return;


	// 카메라 위치, 방향
	FVector CameraLocation;
	FRotator CameraRotation;

	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector ShotDirection = CameraRotation.Vector();
	const float TraceDistance = 10000.f; // 100m 정도

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + ShotDirection * TraceDistance;

	// 서버에 요청
	ServerFire(TraceStart, TraceEnd);
}

void ACatchMePolice::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//EIC->BindAction("Fire", IE_Pressed, this, &ACatchMePolice::Fire);
	EIC->BindAction(FireAction, ETriggerEvent::Triggered, this, &ThisClass::Fire);
}


void ACatchMePolice::ServerFire_Implementation(const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& TraceEnd)
{
    if (!CanFire())
        return;

    const float Now = GetWorld()->GetTimeSeconds();
    NextAllowedFireTime = Now + FireInterval;


    // 여기서 라인트레이스/데미지 처리


    // 무엇을 맞췄는지를 HitResult(구조체)에 저장
    FHitResult HitResult;
    // FCollisionQueryParams(const FName& InTraceTag, bool bInTraceComplex, const AActor* InIgnoreActor);
    // 첫 번째 인자 : 라인 트레이스에 붙을 태그 이름, 두 번째 인자 : 복합 콜리전 사용 여부, 세 번째 인자 : 무시할 액터
    FCollisionQueryParams Params(SCENE_QUERY_STAT(WeaponTrace), false, this);
    // 물리 머티리얼 정보 가져올지 여부
    Params.bReturnPhysicalMaterial = false;

    UWorld* World = GetWorld();
    if (!World)
        return;


    bool bHit = World->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_Visibility,
        Params
    );

    if (bHit)
    {
        AActor* HitActor = HitResult.GetActor();
        UE_LOG(LogTemp, Log, TEXT("[HitScan] Hit actor: %s, Loc: %s"),
            HitActor ? *HitActor->GetName() : TEXT("None"),
            *HitResult.ImpactPoint.ToString());

        // 여기에 나중에 Damage 처리 넣으면 됨
        float DamageAmount = 150.f;
        UGameplayStatics::ApplyDamage(HitActor, DamageAmount, GetController(), this, UDamageType::StaticClass());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("[HitScan] No hit"));
    }

    // 디버그 라인 (서버 월드에서 그림. 원하면 Multicast로 클라에도 그림 표시 가능)
    DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
}

bool ACatchMePolice::CanFire()
{
    if (!HasAuthority())
    {
        // 서버에서만 진짜 검증할 거면 여기선 false만 리턴해도 됨
        return true;
    }

    const float Now = GetWorld()->GetTimeSeconds();
    return Now >= NextAllowedFireTime;
}

