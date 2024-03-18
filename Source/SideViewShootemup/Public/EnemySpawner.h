#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ShipAIController.h"

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
    UPROPERTY()
    TObjectPtr<AShipAIController> ShipAIController;

    UPROPERTY()
    TObjectPtr<AShipPawn> Ship;

    virtual void OnWorldBeginPlay(UWorld& InWorld);

    virtual TStatId GetStatId() const override;
};
