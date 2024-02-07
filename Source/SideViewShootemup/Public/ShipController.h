// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "ShipController.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipController : public APlayerController
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext * ShipInputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction * ThrustAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction * ThrustVectorAction;

    virtual void SetupInputComponent() override;
    virtual void BeginPlay() override;
    virtual void PlayerTick(float DeltaTime) override;

    FVector GetMouseWorldPosition(double planeY);

    void OnTrustTriggered();
    void OnTrustVectorTriggered(const FInputActionInstance& thrustVector);
};
