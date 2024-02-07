// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPawn.h"
#include "SideViewShootemup/SideViewShootemup.h"

AShipPawn::AShipPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainBody"));
    check(MainBody);
    RootComponent = MainBody;
}

void AShipPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AShipPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    GenerateThrust.Broadcast(MainBody, mThrustVector, mThrust, DeltaTime);
    AimAt.Broadcast(mAimPosition, DeltaTime);

    mThrust = 0.0f;
    mThrustVector = FVector::UnitZ();
}

double CalcNewPitch(const FVector& current, const FVector& requested, double maxDelta)
{
    double currentPitch = FMath::RadiansToDegrees(FMath::Atan2(current.Z, current.X));
    double requestedPitch = FMath::RadiansToDegrees(FMath::Atan2(requested.Z, requested.X));
    double delta = FMath::FindDeltaAngleDegrees(currentPitch, requestedPitch);
    return FMath::Abs(delta) > maxDelta ? currentPitch + copysign(maxDelta, delta) : requestedPitch;
}
