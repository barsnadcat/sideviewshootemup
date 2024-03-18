#include "EnemySpawner.h"
#include "Ship/ShipPawn.h"
#include "MyGameMode.h"
#include "GameFramework/GameModeBase.h"

void UEnemySpawner::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
    if (AMyGameMode* const gameMode = InWorld.GetAuthGameMode<AMyGameMode>())
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        SpawnInfo.ObjectFlags |= RF_Transient;
        Ship = InWorld.SpawnActor<AShipPawn>(gameMode->ShipPawnClass, { 1000, 0, 500 }, FRotator::ZeroRotator, SpawnInfo);
    }
}

TStatId UEnemySpawner::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UEnemySpawner, STATGROUP_Tickables);
}
