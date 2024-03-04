#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ShipAIController.h"

#include "EnemySpawner.generated.h"

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

    virtual void OnWorldBeginPlay(UWorld& InWorld);

    virtual TStatId GetStatId() const override;
};
