#include "Ship/BridgeShipPart.h"

#include "GameFramework/PlayerController.h"
#include "Ship/ShipPawn.h"
#include "ShipAIController.h"

ABridgeShipPart::ABridgeShipPart()
{
    BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxCollisionComponent->InitBoxExtent(FVector(40.0f, 40.0f, 40.0f));
    BoxCollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
    BoxCollisionComponent->SetupAttachment(RootComponent);
}

void ABridgeShipPart::BeginPlay()
{
    Super::BeginPlay();
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnInfo.ObjectFlags |= RF_Transient;
    ShipAutoPilot = GetWorld()->SpawnActor<AShipAIController>(ShipAIControllerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
}

void ABridgeShipPart::Interact(APlayerController* playerController)
{
    if (AShipPawn* ship = Ship.Get())
    {
        check(playerController);
        playerController->Possess(ship);
    }
}
