// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "SideViewShootemup/SideViewShootemup.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ShipPawn.h"
#include "MyGameMode.h"

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        inputComponent->BindAction(ThrustVectorAction, ETriggerEvent::Triggered, this, &AMyPlayerController::OnTrustVectorTriggered);
        inputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AMyPlayerController::OnShootTriggered);
        inputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMyPlayerController::OnInteractTriggered);
        inputComponent->BindAction(CancelInteractionAction, ETriggerEvent::Triggered, this, &AMyPlayerController::OnCancelInteractionTriggered);
        inputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::OnMoveTriggered);
    }
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if (AShipPawn* ship = GetPawn<AShipPawn>())
    {
        FVector mouse = GetMouseWorldPosition(ship->GetActorLocation().Y);
        ship->SetAimPosition(mouse);
        DrawDebugPoint(GetWorld(), mouse, 2.0f, FColor::Red);
    }
}

FVector AMyPlayerController::GetMouseWorldPosition(double planeY)
{
    ULocalPlayer* localPlayer = GetLocalPlayer();
    if (!localPlayer || !localPlayer->ViewportClient)
    {
        return FVector::Zero();
    }

    FVector2D mouseScreenPosition = FVector2D::Zero();
    if (!localPlayer->ViewportClient->GetMousePosition(mouseScreenPosition))
    {
        return FVector::Zero();
    }

    FVector deprojectLocation = FVector::Zero();
    FVector deprojectDirection = FVector::Zero();
    if (!DeprojectScreenPositionToWorld(mouseScreenPosition.X, mouseScreenPosition.Y, deprojectLocation, deprojectDirection))
    {
        return FVector::Zero();
    }

    FVector worldMousePosition = FVector::Zero();
    worldMousePosition.Y = planeY;
    double distanceToPlaneY = planeY - deprojectLocation.Y;

    worldMousePosition.X = deprojectLocation.X;
    if (!FMath::IsNearlyZero(deprojectDirection.X))
    {
        worldMousePosition.X = deprojectLocation.X + (distanceToPlaneY * deprojectDirection.X) / deprojectDirection.Y;
    }

    worldMousePosition.Z = deprojectLocation.Z;
    if (!FMath::IsNearlyZero(deprojectDirection.Z))
    {
        worldMousePosition.Z = deprojectLocation.Z + (distanceToPlaneY * deprojectDirection.Z) / deprojectDirection.Y;
    }

    return worldMousePosition;
}

void AMyPlayerController::OnTrustVectorTriggered(const FInputActionInstance& thrustVectorAction)
{
    FVector2D thrustVector = thrustVectorAction.GetValue().Get<FVector2D>();
    //UE_LOG(Game, Display, TEXT("OnTrustVectorTriggered %s"), *thrustVector.ToString());
    if (AShipPawn* ship = GetPawn<AShipPawn>())
    {
        ship->SetThrustVector({ thrustVector.X, 0.0f, thrustVector.Y });
        ship->SetThrust(1.0f);
    }
}

void AMyPlayerController::OnMoveTriggered(const FInputActionInstance& thrustVectorAction)
{
    FVector2D vector = thrustVectorAction.GetValue().Get<FVector2D>();
    UE_LOG(Game, Display, TEXT("OnMoveTriggered %s"), *vector.ToString());

    if (GetPawn() == DefaultPawn)
    {
        GetPawn()->AddMovementInput(FVector(vector.X, 0.0f, vector.Y));
    }
}

void AMyPlayerController::OnShootTriggered()
{
    if (AShipPawn* ship = GetPawn<AShipPawn>())
    {
        ship->Shoot.Broadcast();
    }
}

void AMyPlayerController::OnInteractTriggered()
{
    if (AMyGameMode* const gameMode = GetWorld()->GetAuthGameMode<AMyGameMode>())
    {
        if (gameMode->ShipPawn)
        {
            Possess(gameMode->ShipPawn);
        }
    }
}

void AMyPlayerController::OnCancelInteractionTriggered()
{
    Possess(DefaultPawn);
    if (AMyGameMode* const gameMode = GetWorld()->GetAuthGameMode<AMyGameMode>())
    {
        gameMode->ShipAutoPilot->Possess(gameMode->ShipPawn);
    }
}

void AMyPlayerController::SetPawn(APawn* inPawn)
{
    Super::SetPawn(inPawn);
    UEnhancedInputLocalPlayerSubsystem* inputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (!inputSubsystem || !inPawn)
    {
        return;
    }

    if (inPawn != DefaultPawn)
    {
        inputSubsystem->RemoveMappingContext(CharacterInputMappingContext);
        if (!inputSubsystem->HasMappingContext(ShipInputMappingContext))
        {
            inputSubsystem->AddMappingContext(ShipInputMappingContext, 0);
        }
    }
    else
    {
        inputSubsystem->RemoveMappingContext(ShipInputMappingContext);
        if (!inputSubsystem->HasMappingContext(CharacterInputMappingContext))
        {
            inputSubsystem->AddMappingContext(CharacterInputMappingContext, 0);
        }
    }
}

void AMyPlayerController::SetDefaultPawn(APawn* defaultPawn)
{
    check(defaultPawn);
    DefaultPawn = defaultPawn;
}
