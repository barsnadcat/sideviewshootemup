#include "Ship/ShipConstraint.h"

#include "PhysicsEngine/PhysicsConstraintComponent.h"

AShipConstraint::AShipConstraint()
{
    PrimaryActorTick.bCanEverTick = false;
    Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
    RootComponent = Constraint;
}
