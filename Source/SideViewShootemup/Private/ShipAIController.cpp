// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipAIController.h"

void AShipAIController::BeginPlay()
{
    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
    }
}
