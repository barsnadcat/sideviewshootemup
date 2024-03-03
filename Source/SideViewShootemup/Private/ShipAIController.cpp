// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipAIController.h"

void AShipAIController::OnPossess(APawn* inPawn)
{
    Super::OnPossess(inPawn);
    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
    }
}
