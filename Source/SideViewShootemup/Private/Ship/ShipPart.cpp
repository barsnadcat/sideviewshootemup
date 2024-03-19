#include "Ship/ShipPart.h"

#include "Ship/ShipPawn.h"

AShipPart::AShipPart()
{
    PrimaryActorTick.bCanEverTick = false;
    MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainBody"));
    RootComponent = MainBody;
    Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
    Constraint->SetupAttachment(MainBody);
}

void AShipPart::Attach(AShipPart* parent)
{
    Constraint->ConstraintActor1 = this;
    Constraint->ConstraintActor2 = parent;
    Constraint->InitComponentConstraint();
}

inline void AShipPart::SetShip(AShipPawn* ship)
{
    Ship = ship;
    OnSetShip(ship);
}
