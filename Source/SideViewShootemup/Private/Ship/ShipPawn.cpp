#include "Ship/ShipPawn.h"

#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Ship/BridgeShipPart.h"
#include "Ship/ShipDesign.h"
#include "Ship/ShipPart.h"
#include "ShipAIController.h"
#include "SideViewShootemup/SideViewShootemup.h"

AShipPawn::AShipPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AShipPawn::BeginPlay()
{
    Super::BeginPlay();
    ConstructShip();
    AutoPilot();
}

void AShipPawn::ConstructShip()
{
    check(ShipDesign);
    UWorld* world = GetWorld();
    check(world);

    // Spawn parts
    FActorSpawnParameters spawnParams;
    spawnParams.bNoFail = true;
    spawnParams.Owner = this;

    TArray<TArray<AShipPart*>> parts;
    parts.SetNum(ShipDesign->X.Num());
    for (uint8 row = 0; row < ShipDesign->X.Num(); row++)
    {
        const auto& designRow = ShipDesign->X[row].Y;
        auto& partsRow = parts[row];
        partsRow.SetNum(designRow.Num());

        for (uint8 column = 0; column < designRow.Num(); column++)
        {
            TSubclassOf<AShipPart> partClass = designRow[column];
            if (partClass)
            {
                FVector partPos = GetActorLocation() + FVector(column * ShipDesign->GridSize, 0.f, row * -ShipDesign->GridSize);
                AShipPart* newPart = world->SpawnActor<AShipPart>(partClass, partPos, FRotator::ZeroRotator, spawnParams);
                newPart->SetCoord(row, column);
                if (ABridgeShipPart* bridgePart = Cast<ABridgeShipPart>(newPart))
                {
                    Bridge = bridgePart;
                }
                partsRow[column] = newPart;
            }
        }
    }

    // Weld parts
    for (uint8 row = 0; row < parts.Num(); row++)
    {
        auto& partsRow = parts[row];
        for (uint8 column = 0; column < partsRow.Num(); column++)
        {
            if (AShipPart* part = partsRow[column])
            {
                if (part != Bridge)
                {
                    part->Body->AttachToComponent(Bridge->Body, {EAttachmentRule::KeepWorld, true});
                }
                if (column > 0)
                {
                    AShipPart::ConnectHorizontally(partsRow[column - 1], part);
                }
                if (row > 0)
                {
                    AShipPart::ConnectVertically(parts[row - 1][column], part);
                }
            }
        }
    }
    UE_LOG(Game, Display, TEXT("Children %d"), Bridge->Body->GetAttachChildren().Num());
}

void AShipPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateThrust.Broadcast(mThrustVector, mThrust, DeltaTime);
    UpdateAimTarget.Broadcast(mAimPosition, DeltaTime);

    mThrust = 0.0f;
    mThrustVector = FVector::UnitZ();
}

void AShipPawn::AutoPilot()
{
    if (IsValid(Bridge))
    {
        Bridge->GetAutoPilot()->Possess(this);
    }
}
