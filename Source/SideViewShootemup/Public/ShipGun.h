// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ShipGun.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIDEVIEWSHOOTEMUP_API UShipGun : public USceneComponent
{
    GENERATED_BODY()

public:
    UShipGun();

    void OnAimAt(const FVector& target, float deltaTime);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    double GunTraking = 180.0f;

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
