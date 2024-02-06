// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShipPawn.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FGenerateThrust, UPrimitiveComponent*, const FVector &, double);

UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AShipPawn();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    UStaticMeshComponent * MainBody;

    FGenerateThrust GenerateThrust;
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void SetThrustVector(const FVector& vector) { mThrustVector = vector; }
    void SetThrust(float thrust) { mThrust = thrust; }
private:
    FVector mThrustVector = FVector::UnitZ();
    float mThrust = 0.0f;
};

