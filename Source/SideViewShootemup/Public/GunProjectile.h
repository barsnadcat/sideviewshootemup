// Fill out your copyright notice in the Description page of Project Settings.

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
    // Sets default values for this actor's properties
    AGunProjectile();

    /** Sphere collision component */
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    TObjectPtr<USphereComponent> CollisionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
