#include "Ship/EngineShipPart.h"
#include "Ship/ShipPawn.h"

AEngineShipPart::AEngineShipPart()
{
    PrimaryActorTick.bCanEverTick = false;
    Axis = CreateDefaultSubobject<UAxisComponent>(TEXT("Axis"));
    Axis->SetupAttachment(MainBody);
}


void AEngineShipPart::OnAttach()
{
    if (AShipPawn* ship = Ship.Get())
    {
        ship->GenerateThrust.AddUObject(this, &AEngineShipPart::OnUpdateThrust);
    }
}

void AEngineShipPart::OnUpdateThrust(UPrimitiveComponent* primitive, const FVector& vector, double thrust, float deltaTime)
{
    check(primitive);
    
    Axis->UpdateDirection(vector, deltaTime);

    if (thrust != 0.0f)
    {
        primitive->AddForceAtLocation(Axis->GetComponentRotation().Vector() * thrust * MaxThrust, Axis->GetComponentLocation());
    }
}
