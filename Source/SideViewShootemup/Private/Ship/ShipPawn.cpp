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
    for (int row = 0; row < parts.Num(); row++)
    {
        auto& partsRow = parts[row];
        for (int col = 0; col < partsRow.Num(); col++)
        {
            if (AShipPart* part = partsRow[col])
            {
                if (col > 0)
                {
                    AShipPart::Weld(part, partsRow[col - 1]);
                }
                if (row > 0)
                {
                    AShipPart::Weld(part, parts[row - 1][col]);
                }
                if (row > 0 && col > 0)
                {
                    AShipPart::Weld(part, parts[row - 1][col - 1]);
                }
                if (row > 0 && col + 1 < parts.Num())
                {
                     AShipPart::Weld(part, parts[row - 1][col + 1]);
                }
            }
        }
    }
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
