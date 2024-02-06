// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ShipEngine.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIDEVIEWSHOOTEMUP_API UShipEngine : public USceneComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UShipEngine();
    void OnGenerateThrust(UPrimitiveComponent* primitive, const FVector& vector, double thrust);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    double MaxThrust = 2000000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    double EngineTraking = 5.0f;
};

double CalcNewPitch(const FVector& current, const FVector& requested, double maxDelta);
