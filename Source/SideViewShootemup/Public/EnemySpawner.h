// Fill out your copyright notice in the Description page of Project Settings.

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
    virtual void OnWorldBeginPlay(UWorld& InWorld);

    virtual TStatId GetStatId() const override;

    UPROPERTY()
    TObjectPtr<AShipAIController> ShipAIController;
};
