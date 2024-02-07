// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipGun.h"
#include "ShipPawn.h"
#include "CalcNewPitch.h"

UShipGun::UShipGun()
{
    PrimaryComponentTick.bCanEverTick = true;

    if (AShipPawn* ship = GetOwner<AShipPawn>())
    {
        ship->AimAt.AddUObject(this, &UShipGun::OnAimAt);
    }
}

void UShipGun::OnAimAt(const FVector& target, float deltaTime)
{
    FVector direction = target - GetComponentLocation();
    if (direction.SquaredLength() > 1.0f)
    {
        double newPitch = CalcNewPitch(GetComponentRotation().Vector(), direction, GunTraking * deltaTime);
        SetWorldRotation(FRotator(newPitch, 0.0f, 0.0f).Quaternion());
    }
}

void UShipGun::BeginPlay()
{
    Super::BeginPlay();
}

void UShipGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

