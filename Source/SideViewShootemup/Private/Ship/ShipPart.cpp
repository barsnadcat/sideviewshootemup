#include "Ship/ShipPart.h"
#include "Ship/ShipPawn.h"

AShipPart::AShipPart()
{
	PrimaryActorTick.bCanEverTick = false;
	MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainBody"));
	RootComponent = MainBody;
	Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
}

void AShipPart::Attach(FVector pos, AShipPart* parent, AShipPawn* ship)
{
	check(ship);
	Ship = ship;
    MainBody->SetRelativeLocation(pos);
	MainBody->AttachToComponent(parent->MainBody, FAttachmentTransformRules::KeepRelativeTransform);
    Constraint->ConstraintActor1 = this;
    Constraint->ConstraintActor2 = parent;
	Constraint->InitComponentConstraint();
	OnAttach(ship);
}

inline void AShipPart::SetShip(AShipPawn* ship) 
{
	Ship = ship; 
}


