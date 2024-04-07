#include "Ship/BridgeShipPart.h"

#include "Character/PhysCharacterPawn.h"
#include "GameFramework/PlayerController.h"
#include "Ship/ShipPawn.h"
#include "ShipAIController.h"

ABridgeShipPart::ABridgeShipPart()
{
}

void ABridgeShipPart::BeginPlay()
{
    Super::BeginPlay();
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnInfo.ObjectFlags |= RF_Transient;
    ShipAutoPilot = GetWorld()->SpawnActor<AShipAIController>(ShipAIControllerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
}

void ABridgeShipPart::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    Super::EndPlay(endPlayReason);
    if (ShipAutoPilot)
    {
        ShipAutoPilot->Destroy();
    }
}

void ABridgeShipPart::Interact(APlayerController* playerController)
{
    if (AShipPawn* ship = GetOwner<AShipPawn>())
    {
        APhysCharacterPawn* pawn = playerController->GetPawn<APhysCharacterPawn>();
        if (pawn)
        {
            pawn->Attach(this);
        }
        check(playerController);
        playerController->Possess(ship);
    }
}
