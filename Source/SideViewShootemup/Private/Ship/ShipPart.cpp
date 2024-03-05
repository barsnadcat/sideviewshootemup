#include "Ship/ShipPart.h"

AShipPart::AShipPart()
{
	PrimaryActorTick.bCanEverTick = false;
	MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainBody"));
	RootComponent = MainBody;
}


