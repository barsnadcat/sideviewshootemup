#include "Ship/ShipPawn.h"

#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Ship/BridgeShipPart.h"
#include "Ship/ShipDesign.h"
#include "Ship/ShipPart.h"
#include "ShipAIController.h"
#include "SideViewShootemup/SideViewShootemup.h"
#include "Ship/MyClusterUnionComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AShipPawn::AShipPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    ClusterUnion = CreateDefaultSubobject<UMyClusterUnionComponent>(TEXT("ClusterUnion"));
    RootComponent = ClusterUnion;
}

void AShipPawn::BeginPlay()
{
    Super::BeginPlay();
    if (ShipDesign)
    {
        ConstructShip(ShipDesign);
        AutoPilot();
    }
}

void AShipPawn::Union(AShipPart* shipPart)
{
    SCOPED_NAMED_EVENT_TEXT("AShipPawn::Union", FColor::Red);
    ClusterUnion->AddComponentToCluster(shipPart->GeometryCollection, {});
}

void AShipPawn::UnUnion(AShipPart* shipPart)
{
    SCOPED_NAMED_EVENT_TEXT("AShipPawn::UnUnion", FColor::Red);
    check(ClusterUnion);
    check(shipPart);
    check(shipPart->GeometryCollection);
    ClusterUnion->RemoveComponentFromCluster(shipPart->GeometryCollection);
    shipPart->DisablePart();
}

void AShipPawn::ConstructShip(UShipDesign* design)
{
    check(design);
    UWorld* world = GetWorld();
    check(world);

    // Spawn parts
    FActorSpawnParameters spawnParams;
    spawnParams.bNoFail = true;
    spawnParams.Owner = this;

    TArray<TArray<AShipPart*>> parts;
    parts.SetNum(design->X.Num());
    for (uint8 row = 0; row < design->X.Num(); row++)
    {
        const auto& designRow = design->X[row].Y;
        auto& partsRow = parts[row];
        partsRow.SetNum(designRow.Num());

        for (uint8 column = 0; column < designRow.Num(); column++)
        {
            TSubclassOf<AShipPart> partClass = designRow[column];
            if (partClass)
            {
                FVector partPos = GetActorLocation() + FVector(column * design->GridSize, 0.f, row * -design->GridSize);
                AShipPart* newPart = world->SpawnActor<AShipPart>(partClass, partPos, FRotator::ZeroRotator, spawnParams);
                newPart->SetCoord(row, column);
                if (ABridgeShipPart* bridgePart = Cast<ABridgeShipPart>(newPart))
                {
                    Bridge = bridgePart;
                }
                Union(newPart);
                partsRow[column] = newPart;
            }
        }
    }

    // Connect parts
    for (uint8 row = 0; row < parts.Num(); row++)
    {
        auto& partsRow = parts[row];
        for (uint8 column = 0; column < partsRow.Num(); column++)
        {
            if (AShipPart* part = partsRow[column])
            {
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
    //UE_LOG(Game, Display, TEXT("Children %d"), Bridge->Body->GetAttachChildren().Num());
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

void AShipPawn::ConstructShip(TSet<AShipPart*>& parts)
{
    SCOPED_NAMED_EVENT_TEXT("AShipPawn::ConstructShip", FColor::Red);
    for (AShipPart* part : parts)
    {
        if (!Bridge)
        {
            Bridge = part;
        }
        Union(part);
    }
}
