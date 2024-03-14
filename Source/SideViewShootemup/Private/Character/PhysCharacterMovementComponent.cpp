// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PhysCharacterMovementComponent.h"

void UPhysCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    FVector InputVector = ConsumeInputVector();

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!UpdatedPrimitive)
    {
        return;
    }
    Acceleration = InputVector * 2000.0f;

    UpdatedPrimitive->AddForce(Acceleration, NAME_None, true);

    FRotator orientation;

    if (Acceleration.X > 0.0f)
    {
        orientation.Yaw = 0.0f;
        UpdatedPrimitive->SetWorldRotation(orientation);
    }
    else if (Acceleration.X < 0.0f)
    {
        orientation.Yaw = 180.0f;
        UpdatedPrimitive->SetWorldRotation(orientation);
    }
}
