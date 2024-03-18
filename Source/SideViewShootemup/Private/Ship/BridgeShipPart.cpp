#include "Ship/BridgeShipPart.h"
#include "Ship/ShipPawn.h"
#include "GameFramework/PlayerController.h"



ABridgeShipPart::ABridgeShipPart()
{
    BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxCollisionComponent->InitBoxExtent(FVector(40.0f, 40.0f, 40.0f));
    BoxCollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
    BoxCollisionComponent->SetupAttachment(RootComponent);
}

void ABridgeShipPart::Interact(APlayerController* playerController)
{
    if (AShipPawn* ship = Ship.Get())
    {
        check(playerController);
        playerController->Possess(ship);
    }
}
