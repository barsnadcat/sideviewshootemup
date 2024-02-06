// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPawn.h"
#include "SideViewShootemup/SideViewShootemup.h"

AShipPawn::AShipPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainBody"));
    check(MainBody);
    RootComponent = MainBody;
    EngineHandle = CreateDefaultSubobject<USceneComponent>(TEXT("EngineHandle"));
    check(EngineHandle);
    EngineAxis = CreateDefaultSubobject<USceneComponent>(TEXT("EngineAxis"));
    check(EngineAxis);
    EngineHandle->SetupAttachment(MainBody);
    EngineAxis->SetupAttachment(EngineHandle);
}

void AShipPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AShipPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    check(IsValid(EngineAxis));
    double newPitch = CalcNewPitch(EngineAxis->GetComponentRotation().Vector(), mThrustVector, EngineTraking);
    EngineAxis->SetWorldRotation(FRotator(newPitch, 0.0f, 0.0f).Quaternion());

    if (mThrust != 0.0f)
    {
        check(IsValid(MainBody));
        MainBody->AddForce(EngineAxis->GetComponentRotation().Vector() * mThrust * ThrustUnit);
    }

    mThrust = 0.0f;
    mThrustVector = FVector::UnitZ();
}

void AShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

double CalcNewPitch(const FVector& current, const FVector& requested, double maxDelta)
{
    double currentPitch = FMath::RadiansToDegrees(FMath::Atan2(current.Z, current.X));
    double requestedPitch = FMath::RadiansToDegrees(FMath::Atan2(requested.Z, requested.X));
    double delta = FMath::FindDeltaAngleDegrees(currentPitch, requestedPitch);
    return FMath::Abs(delta) > maxDelta ? currentPitch + copysign(maxDelta, delta) : requestedPitch;
}
