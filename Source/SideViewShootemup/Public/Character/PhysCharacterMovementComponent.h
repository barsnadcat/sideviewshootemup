// Copyright 2023 FragLab Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PhysCharacterMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API UPhysCharacterMovementComponent : public UPawnMovementComponent
{
    GENERATED_BODY()
public:
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

    UPROPERTY(EditAnywhere)
    double AccelerationScale = 2000.f;

    FVector Acceleration;
};
