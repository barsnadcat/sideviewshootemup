// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipEngine.h"
#include "ShipPawn.h"

// Sets default values for this component's properties
UShipEngine::UShipEngine()
{
    PrimaryComponentTick.bCanEverTick = false;

    if (AShipPawn* ship = GetOwner<AShipPawn>())
    {
        ship->GenerateThrust.AddUObject(this, &UShipEngine::OnGenerateThrust);
    }
}

void UShipEngine::OnGenerateThrust(UPrimitiveComponent* primitive, const FVector& vector, double thrust, float deltaTime)
{
    check(IsValid(primitive));

    double newPitch = CalcNewPitch(GetComponentRotation().Vector(), vector, EngineTraking * deltaTime);
    SetWorldRotation(FRotator(newPitch, 0.0f, 0.0f).Quaternion());

    if (thrust != 0.0f)
    {
        primitive->AddForceAtLocation(GetComponentRotation().Vector() * thrust * MaxThrust, GetComponentLocation());
    }
}

double CalcNewPitch(const FVector& current, const FVector& requested, double maxDelta)
{
    double currentPitch = FMath::RadiansToDegrees(FMath::Atan2(current.Z, current.X));
    double requestedPitch = FMath::RadiansToDegrees(FMath::Atan2(requested.Z, requested.X));
    double delta = FMath::FindDeltaAngleDegrees(currentPitch, requestedPitch);
    return FMath::Abs(delta) > maxDelta ? currentPitch + copysign(maxDelta, delta) : requestedPitch;
}

