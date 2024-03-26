#include "Ship/ShipPart.h"

#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Ship/ShipPawn.h"
#include "HealthComponent.h"
#include "SideViewShootemup/SideViewShootemup.h"

AShipPart::AShipPart()
{
    PrimaryActorTick.bCanEverTick = false;
    MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainBody"));
    RootComponent = MainBody;
    Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}
