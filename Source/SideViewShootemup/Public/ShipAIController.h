// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShipAIController.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipAIController : public AAIController
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = AI)
    TObjectPtr<UBehaviorTree> BehaviorTree;
    virtual void OnPossess(APawn* InPawn) override;
};
