// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShipPawn.generated.h"

UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AShipPawn();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UStaticMeshComponent * MainBody;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void SetThrustVector(const FVector2D& vector) { mThrustVector = vector; }
    void SetThrust(float thrust) { mThrust = thrust; }
private:
    FVector2D mThrustVector = FVector2D::Zero();
    float mThrust = 1000000.0f;
};
