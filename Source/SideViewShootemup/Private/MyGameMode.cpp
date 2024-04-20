#include "MyGameMode.h"

#include "MyPlayerController.h"
#include "Ship/ShipPawn.h"
#include "ShipAIController.h"

APawn* AMyGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
    APawn* result = Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
    AMyPlayerController* myController = CastChecked<AMyPlayerController>(NewPlayer);
    myController->SetDefaultPawn(result);
    return result;
}

void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnInfo.ObjectFlags |= RF_Transient;
    ShipPawn = GetWorld()->SpawnActor<AShipPawn>(ShipPawnClass, {-6000, 0, 6000}, FRotator::ZeroRotator, SpawnInfo);
}
