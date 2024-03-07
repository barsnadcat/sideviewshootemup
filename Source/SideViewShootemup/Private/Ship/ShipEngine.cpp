#include "Ship/ShipEngine.h"
#include "Ship/ShipPawn.h"
#include "CalcNewPitch.h"

// Sets default values for this component's properties
UShipEngine::UShipEngine()
{
    PrimaryComponentTick.bCanEverTick = false;

    if (AShipPart* part = GetOwner<AShipPart>())
    {
        part->OnConnectToShip.AddUObject(this, &UShipEngine::OnConnectToShip);
    }
}

void UShipEngine::OnConnectToShip(AShipPawn* ship)
{
    check(ship);
    ship->GenerateThrust.AddUObject(this, &UShipEngine::OnGenerateThrust);
}

void UShipEngine::OnGenerateThrust(UPrimitiveComponent* primitive, const FVector& vector, double thrust, float deltaTime)
{
    check(primitive);

    double newPitch = CalcNewPitch(GetComponentRotation().Vector(), vector, EngineTraking * deltaTime);
    SetWorldRotation(FRotator(newPitch, 0.0f, 0.0f).Quaternion());

    if (thrust != 0.0f)
    {
        primitive->AddForceAtLocation(GetComponentRotation().Vector() * thrust * MaxThrust, GetComponentLocation());
    }
}
