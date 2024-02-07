// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipGun.h"
#include "ShipPawn.h"
#include "CalcNewPitch.h"

UShipGun::UShipGun()
{
    PrimaryComponentTick.bCanEverTick = true;

    GunMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("GunMuzzle"));
    GunMuzzle->SetupAttachment(this);

    if (AShipPawn* ship = GetOwner<AShipPawn>())
    {
        ship->AimAt.AddUObject(this, &UShipGun::OnAimAt);
        ship->Shoot.AddUObject(this, &UShipGun::OnShoot);
    }
}

void UShipGun::OnAimAt(const FVector& target, float deltaTime)
{
    FVector direction = target - GetComponentLocation();
    if (direction.SquaredLength() > 1.0f)
    {
        double newPitch = CalcNewPitch(GetComponentRotation().Vector(), direction, GunTraking * deltaTime);
        SetWorldRotation(FRotator(newPitch, 0.0f, 0.0f).Quaternion());
    }
}

void UShipGun::OnShoot()
{
    if (!ProjectileClass)
    {
        return;
    }
    UWorld* World = GetValid(GetWorld());
    if (!World)
    {
        return;
    }
    if (!GunMuzzle)
    {
        return;
    }

    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    // Spawn the projectile at the muzzle
    World->SpawnActor(ProjectileClass, &GunMuzzle->GetComponentTransform(), ActorSpawnParams);
}

void UShipGun::BeginPlay()
{
    Super::BeginPlay();
}

void UShipGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
