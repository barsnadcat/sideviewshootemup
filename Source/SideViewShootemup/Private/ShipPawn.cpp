// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPawn.h"

// Sets default values
AShipPawn::AShipPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainBody"));
}

// Called when the game starts or when spawned
void AShipPawn::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AShipPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsValid(MainBody))
    {
        FVector force = { mThrustVector.X, 0.0f, mThrustVector.Y };
        MainBody->AddForce(force * mThrust);
    }

    mThrustVector = FVector2D::Zero();
}

// Called to bind functionality to input
void AShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
