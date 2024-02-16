// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"
#include "ShipPawn.h"
#include "GameFramework/GameModeBase.h"

void UEnemySpawner::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnInfo.ObjectFlags |= RF_Transient;
    ShipAIController = InWorld.SpawnActor<AShipAIController>(AShipAIController::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
    AGameModeBase* const gameMode = InWorld.GetAuthGameMode();
    if (gameMode)
    {
        AShipPawn* ship = InWorld.SpawnActor<AShipPawn>(gameMode->DefaultPawnClass, { 300, 0, 200 }, FRotator::ZeroRotator, SpawnInfo);
        ShipAIController->Possess(ship);
    }
}

TStatId UEnemySpawner::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UEnemySpawner, STATGROUP_Tickables);
}
