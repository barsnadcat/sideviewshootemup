#include "Ship/GunShipPart.h"
#include "SideViewShootemup/SideViewShootemup.h"
#include "Ship/ShipPawn.h"

AGunShipPart::AGunShipPart()
{
    PrimaryActorTick.bCanEverTick = false;
    Axis = CreateDefaultSubobject<UAxisComponent>(TEXT("Axis"));
    Axis->SetupAttachment(MainBody);
    GunMuzzle = CreateDefaultSubobject<USceneComponent>(TEXT("GunMuzzle"));
    GunMuzzle->SetupAttachment(Axis);
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

    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

    // Spawn the projectile at the muzzle
    world->SpawnActor(ProjectileClass, &GunMuzzle->GetComponentTransform(), ActorSpawnParams);
    mLastShootSeconds = world->TimeSeconds;
}

void AGunShipPart::OnAttach(AShipPawn* ship)
{
    ship->UpdateAimTarget.AddUObject(this, &AGunShipPart::OnUpdateAimTarget);
    ship->Shoot.AddUObject(this, &AGunShipPart::OnShoot);
}
