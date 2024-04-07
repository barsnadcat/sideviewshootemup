#include "Ship/ShipPawn.h"

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
    for (int x = 0; x < ShipDesign->X.Num(); x++)
    {
        const auto& designRow = ShipDesign->X[x].Y;
        auto& partsRow = parts[x];
        partsRow.SetNum(designRow.Num());

        for (int y = 0; y < designRow.Num(); y++)
        {
            TSubclassOf<AShipPart> partClass = designRow[y];
            if (partClass)
            {
                FVector partPos = GetActorLocation() + FVector(y * -ShipDesign->GridSize, 0.f, x * ShipDesign->GridSize);
                partsRow[y] = world->SpawnActor<AShipPart>(partClass, partPos, FRotator::ZeroRotator, spawnParams);
                if (ABridgeShipPart* bridgePart = Cast<ABridgeShipPart>(partsRow[y]))
                {
                    Bridge = bridgePart;
                }
            }
        }
    }

    // Weld parts
    for (int x = 0; x < parts.Num(); x++)
    {
        auto& partsRow = parts[x];
        for (int y = 0; y < partsRow.Num(); y++)
        {
            if (AShipPart* part = partsRow[y])
            {
                if (part != Bridge)
                {
                    part->MainBody->AttachToComponent(Bridge->MainBody, {EAttachmentRule::KeepWorld, true});
                }
                /*if (y > 0)
                {
                    AShipPart::Weld(part, partsRow[y - 1]);
                }
                if (x > 0)
                {
                    AShipPart::Weld(part, parts[x - 1][y]);
                }*/
            }
        }
    }
    UE_LOG(Game, Display, TEXT("Children %d"), Bridge->MainBody->GetAttachChildren().Num());
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
