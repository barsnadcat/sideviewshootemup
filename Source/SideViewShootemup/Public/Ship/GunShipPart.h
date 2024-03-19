#pragma once

#include "CoreMinimal.h"
#include "Ship/AxisComponent.h"
#include "Ship/ShipPart.h"

#include "GunShipPart.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API AGunShipPart : public AShipPart
{
    GENERATED_BODY()
public:
    AGunShipPart();

    UPROPERTY(EditDefaultsOnly, Category = Game)
    TSubclassOf<class AActor> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    TObjectPtr<USceneComponent> GunMuzzle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    TObjectPtr<UAxisComponent> Axis;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    double RPM = 180.f;

private:
    void OnUpdateAimTarget(const FVector& target, float deltaTime);
    void OnShoot();
    virtual void OnSetShip(AShipPawn* ship) override;
    double mLastShootSeconds = 0.0f;
};
