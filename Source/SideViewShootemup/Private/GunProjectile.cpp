#include "GunProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "SideViewShootemup/SideViewShootemup.h"

AGunProjectile::AGunProjectile()
{
    // Use a sphere as a simple collision representation
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(1.0f);

    // Players can't walk on it
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;

    // Set as root component
    RootComponent = CollisionComp;

    // Use a ProjectileMovementComponent to govern this projectile's movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;

    // Die after 3 seconds by default
    InitialLifeSpan = 3.0f;
}

void AGunProjectile::BeginPlay()
{
    Super::BeginPlay();
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGunProjectile::HandleComponentBeginOverlap);
}

void AGunProjectile::HandleComponentBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult)
{
    /* UE_LOG(Game, Display, TEXT("HandleComponentHit: comp:%s other:%s othercomp:%s normal:%s result:%d"),
        HitComp ? *HitComp->GetFName().ToString() : TEXT("null"),
        OtherActor ? *OtherActor->GetFName().ToString() : TEXT("null"),
        OtherComp ? *OtherComp->GetFName().ToString() : TEXT("null"), *NormalImpulse.ToString(), Hit.bBlockingHit);*/

    // Only add impulse and destroy projectile if we hit a physics
    if (otherActor == nullptr || otherComp == nullptr || otherActor == this || otherActor->GetOwner() == GetOwner())
    {
        return;
    }

    APawn* ownerPawn = GetOwner<APawn>();
    UGameplayStatics::ApplyPointDamage(otherActor, Damage, ProjectileMovement->Velocity, sweepResult, ownerPawn ? ownerPawn->Controller : nullptr, this, nullptr);

    if (otherComp->IsSimulatingPhysics())
    {
        otherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
        Destroy();
    }
}