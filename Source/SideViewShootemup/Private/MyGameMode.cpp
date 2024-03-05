#include "MyGameMode.h"
#include "MyPlayerController.h"

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
    ShipPawn = GetWorld()->SpawnActor<AShipPawn>(ShipPawnClass, { 400, 0, 800 }, FRotator::ZeroRotator, SpawnInfo);
    ShipAutoPilot = GetWorld()->SpawnActor<AShipAIController>(ShipAIControllerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
}
