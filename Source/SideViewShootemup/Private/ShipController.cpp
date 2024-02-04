// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipController.h"
#include "SideViewShootemup/SideViewShootemup.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ShipPawn.h"

void AShipController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Set up action bindings
    if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // Setup mouse input events
        inputComponent->BindAction(ThrustAction, ETriggerEvent::Triggered, this, &AShipController::OnTrustTriggered);
        inputComponent->BindAction(ThrustVectorAction, ETriggerEvent::Triggered, this, &AShipController::OnTrustVectorTriggered);

        // Setup touch input events
        //EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AMyProject4PlayerController::OnInputStarted);
        //EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AMyProject4PlayerController::OnTouchTriggered);
        //EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AMyProject4PlayerController::OnTouchReleased);
        //EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AMyProject4PlayerController::OnTouchReleased);
    }
    //else
    {
        //UE_LOG(LogTemplateCharacter, Error, TEXT(" '%s' Failed to find an Enhanced Input Component !This template is built to use the Enhanced Input system.If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
    }
}

void AShipController::BeginPlay()
{
    Super::BeginPlay();
    //Add Input Mapping Context
    if (UEnhancedInputLocalPlayerSubsystem* inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        inputSubsystem->AddMappingContext(ShipInputMappingContext, 0);
    }
}

void AShipController::OnTrustTriggered()
{
    UE_LOG(Game, Display, TEXT("OnTrustTriggered"));
}

void AShipController::OnTrustVectorTriggered(const FInputActionInstance& thrustVectorAction)
{
    FVector2D thrustVector = thrustVectorAction.GetValue().Get<FVector2D>();
    UE_LOG(Game, Display, TEXT("OnTrustVectorTriggered %s"), *thrustVector.ToString());
    AShipPawn* ship = GetPawn<AShipPawn>();
    if (IsValid(ship))
    {
        ship->SetThrustVector(thrustVector);
    }
}
