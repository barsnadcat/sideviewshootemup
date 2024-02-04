// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPawn.h"
#include "SideViewShootemup/SideViewShootemup.h"

AShipPawn::AShipPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainBody"));
    check(MainBody);
    RootComponent = MainBody;
    EngineAxis = CreateDefaultSubobject<USceneComponent>(TEXT("EngineAxis"));
    check(EngineAxis);
    EngineAxis->SetupAttachment(MainBody);
}

void AShipPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AShipPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (mThrust != 0.0f)
    {
        check(IsValid(MainBody));
        MainBody->AddForce(mThrustVector * mThrust * ThrustUnit);
    }

    check(IsValid(EngineAxis));
    EngineAxis->SetRelativeRotation((-mThrustVector).ToOrientationQuat());

    mThrust = 0.0f;
    mThrustVector = FVector::UnitZ();
}

void AShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
