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
    //RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    ClusterUnion = CreateDefaultSubobject<UMyClusterUnionComponent>(TEXT("ClusterUnion"));
    RootComponent = ClusterUnion;
    //ClusterUnion->SetupAttachment(RootComponent);
}

void AShipPawn::BeginPlay()
{
    Super::BeginPlay();
    ConstructShip();
    AutoPilot();
}

void AShipPawn::Union(AShipPart* shipPart)
{
    ClusterUnion->AddComponentToCluster(shipPart->GeometryCollection, {});
    shipPart->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
}

void AShipPawn::UnUnion(AShipPart* shipPart)
{
    check(ClusterUnion);
    check(shipPart);
    check(shipPart->GeometryCollection);
    ClusterUnion->RemoveComponentFromCluster(shipPart->GeometryCollection);
    shipPart->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    shipPart->GeometryCollection->SetEnableDamageFromCollision(false);
    shipPart->DisablePart();
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
