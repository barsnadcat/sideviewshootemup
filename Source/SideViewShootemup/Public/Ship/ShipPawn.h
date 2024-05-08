#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "ShipPawn.generated.h"

class AShipPart;
class ABridgeShipPart;
class UShipDesign;
class UMyClusterUnionComponent; 

DECLARE_MULTICAST_DELEGATE_ThreeParams(FGenerateThrust, const FVector&, double, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FUpdateAimTarget, const FVector&, float);
DECLARE_MULTICAST_DELEGATE(FShoot);

UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipPawn : public APawn
{
    GENERATED_BODY()

public:
    AShipPawn();

    UPROPERTY(EditAnywhere)
    TObjectPtr<UMyClusterUnionComponent> ClusterUnion;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UShipDesign> ShipDesign;

    FGenerateThrust UpdateThrust;
    FUpdateAimTarget UpdateAimTarget;
    FShoot Shoot;

public:
    virtual void BeginPlay() override;
    void Union(AShipPart* a);
    virtual void Tick(float DeltaTime) override;

    void SetAimPosition(const FVector& vector) { mAimPosition = vector; }
    void SetThrustVector(const FVector& vector) { mThrustVector = vector; }
    void SetThrust(float thrust) { mThrust = thrust; }
    void AutoPilot();

private:
    void ConstructShip();

    TObjectPtr<ABridgeShipPart> Bridge;

    FVector mThrustVector = FVector::UnitZ();
    FVector mAimPosition = FVector::UnitX();
    float mThrust = 0.0f;
};
