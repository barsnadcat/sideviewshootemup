// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShipPawn.h"
#include "MyGameMode.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API AMyGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
    TSubclassOf<APawn> ShipPawnClass;
    UPROPERTY()
    TObjectPtr<AShipPawn> ShipPawn;

    virtual void BeginPlay() override;
};
