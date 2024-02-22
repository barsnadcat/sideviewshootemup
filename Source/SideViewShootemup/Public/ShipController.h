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
    TObjectPtr<UInputMappingContext> ShipInputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> ThrustAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> ThrustVectorAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> ShootAction;

    virtual void SetupInputComponent() override;
    virtual void PlayerTick(float DeltaTime) override;
    virtual void SetPawn(APawn* InPawn) override;

    FVector GetMouseWorldPosition(double planeY);

    void OnTrustTriggered();
    void OnTrustVectorTriggered(const FInputActionInstance& thrustVector);
    void OnShootTriggered();
};
