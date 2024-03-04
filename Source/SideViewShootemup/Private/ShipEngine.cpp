#include "ShipEngine.h"
#include "ShipPawn.h"
#include "CalcNewPitch.h"

// Sets default values for this component's properties
UShipEngine::UShipEngine()
{
    PrimaryComponentTick.bCanEverTick = false;

    if (AShipPawn* ship = GetOwner<AShipPawn>())
    {
        ship->GenerateThrust.AddUObject(this, &UShipEngine::OnGenerateThrust);
    }
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
