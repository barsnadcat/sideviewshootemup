// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShipPawn.generated.h"

DECLARE_MULTICAST_DELEGATE_FourParams(FGenerateThrust, UPrimitiveComponent*, const FVector &, double, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FAimAt, const FVector &, float);
DECLARE_MULTICAST_DELEGATE(FShoot);

UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AShipPawn();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    TObjectPtr<UStaticMeshComponent> MainBody;

    FGenerateThrust GenerateThrust;
    FAimAt AimAt;
    FShoot Shoot;
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void SetAimPosition(const FVector& vector) { mAimPosition = vector; }
    void SetThrustVector(const FVector& vector) { mThrustVector = vector; }
    void SetThrust(float thrust) { mThrust = thrust; }
private:
    FVector mThrustVector = FVector::UnitZ();
    FVector mAimPosition = FVector::UnitX();
    float mThrust = 0.0f;
};
