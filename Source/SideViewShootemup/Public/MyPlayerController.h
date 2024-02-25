// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MyPlayerController.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API AMyPlayerController : public APlayerController
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputMappingContext> ShipInputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputMappingContext> CharacterInputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> ThrustVectorAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> ShootAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> InteractAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> CancelInteractionAction;

public:
    virtual void SetupInputComponent() override;
    virtual void PlayerTick(float DeltaTime) override;
    virtual void SetPawn(APawn* InPawn) override;

    void SetDefaultPawn(APawn* defaultPawn);

private:
    void OnTrustVectorTriggered(const FInputActionInstance& thrustVector);
    void OnShootTriggered();
    void OnInteractTriggered();
    void OnCancelInteractionTriggered();

    FVector GetMouseWorldPosition(double planeY);

    UPROPERTY()
    TObjectPtr<APawn> DefaultPawn;
};
