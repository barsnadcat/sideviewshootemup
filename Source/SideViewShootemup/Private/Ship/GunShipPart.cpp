#include "Ship/GunShipPart.h"

#include "Components/BoxComponent.h"
#include "Ship/ShipPawn.h"
#include "SideViewShootemup/SideViewShootemup.h"

AGunShipPart::AGunShipPart()
{
    PrimaryActorTick.bCanEverTick = false;
    Axis = CreateDefaultSubobject<UAxisComponent>(TEXT("Axis"));
    Axis->SetupAttachment(RootComponent);
    GunMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("GunMuzzle"));
    GunMuzzle->SetupAttachment(Axis);
}

void AGunShipPart::DisablePart()
{
    if (AShipPawn* ship = GetOwner<AShipPawn>())
    {
        ship->UpdateAimTarget.Remove(OnUpdateAimTargetHandle);
        ship->Shoot.Remove(OnShootHandle);
    }
    //SetOwner(nullptr);
}

void AGunShipPart::PostActorCreated()
{
    if (AShipPawn* ship = GetOwner<AShipPawn>())
    {
        OnUpdateAimTargetHandle = ship->UpdateAimTarget.AddUObject(this, &AGunShipPart::OnUpdateAimTarget);
        OnShootHandle = ship->Shoot.AddUObject(this, &AGunShipPart::OnShoot);
    }
}

void AGunShipPart::OnUpdateAimTarget(const FVector& target, float deltaTime)
{
    if (target.IsNearlyZero())
    {
        return;
    }
    FVector direction = target - Axis->GetComponentLocation();
    if (!direction.IsNearlyZero())
    {
        Axis->UpdateDirection(direction, deltaTime);
    }
}

void AGunShipPart::OnShoot()
{
    if (!ProjectileClass)
    {
        return;
    }

    UWorld* world = GetWorld();
    check(world);
    if (world->TimeSeconds - mLastShootSeconds < 60.0f / RPM)
    {
        return;
    }

    FActorSpawnParameters spawnParams;
    spawnParams.Owner = GetOwner();
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Spawn the projectile at the muzzle
    world->SpawnActor(ProjectileClass, &GunMuzzle->GetComponentTransform(), spawnParams);
    mLastShootSeconds = world->TimeSeconds;
}
