#pragma once

#include "CoreMinimal.h"
#include "ShipAIController.h"
#include "Subsystems/WorldSubsystem.h"

#include "EnemySpawner.generated.h"

class AShipPawn;

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API UEnemySpawner : public UTickableWorldSubsystem
{
    GENERATED_BODY()
public:
    virtual void OnWorldBeginPlay(UWorld& InWorld);
    virtual TStatId GetStatId() const override;

private:
    TObjectPtr<AShipPawn> Ship;
};
