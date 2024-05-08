#include "Ship/EngineShipPart.h"

#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "PhysicsEngine/ClusterUnionComponent.h"
#include "Ship/ShipPawn.h"
#include "Ship/MyClusterUnionComponent.h"

AEngineShipPart::AEngineShipPart()
{
    PrimaryActorTick.bCanEverTick = false;
    Axis = CreateDefaultSubobject<UAxisComponent>(TEXT("Axis"));
    Axis->SetupAttachment(RootComponent);
}

void AEngineShipPart::PostActorCreated()
{
    if (AShipPawn* ship = GetOwner<AShipPawn>())
    {
        ship->UpdateThrust.AddUObject(this, &AEngineShipPart::OnUpdateThrust);
    }
}

void AEngineShipPart::OnUpdateThrust(const FVector& vector, double thrust, float deltaTime)
{
    Axis->UpdateDirection(vector, deltaTime);
    if (thrust != 0.0f)
    {
        if (AShipPawn* ship = GetOwner<AShipPawn>())
        {
            auto Force = Axis->GetComponentRotation().Vector() * thrust * MaxThrust;
            auto Position = Axis->GetComponentLocation();
            ship->ClusterUnion->AddForceAtLocation(Force, Position, NAME_None);
        }
        //GeometryCollection->AddForceAtLocation(Axis->GetComponentRotation().Vector() * thrust * MaxThrust, Axis->GetComponentLocation());
    }
}
