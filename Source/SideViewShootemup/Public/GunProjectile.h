#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"

#include "GunProjectile.generated.h"

UCLASS()
class SIDEVIEWSHOOTEMUP_API AGunProjectile : public AActor
{
    GENERATED_BODY()

public:
    AGunProjectile();

    virtual void BeginPlay() override;

    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    TObjectPtr<USphereComponent> CollisionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

    UPROPERTY(EditAnywhere, Category = Projectile)
    double Damage = 40.f;

    UFUNCTION()
    void HandleComponentBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool fromSweep, const FHitResult& sweepResult);
};
