#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "ShipPawn.generated.h"

class AShipPart;
class ABridgeShipPart;

DECLARE_MULTICAST_DELEGATE_FourParams(FGenerateThrust, UPrimitiveComponent*, const FVector&, double, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FUpdateAimTarget, const FVector&, float);
DECLARE_MULTICAST_DELEGATE(FShoot);

UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AShipPawn();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
    TSubclassOf<ABridgeShipPart> BridgeClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
    TSubclassOf<AShipPart> SidePartClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
    TSubclassOf<AShipPart> TopPartClass;

    FGenerateThrust UpdateThrust;
    FUpdateAimTarget UpdateAimTarget;
    FShoot Shoot;

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void SetAimPosition(const FVector& vector) { mAimPosition = vector; }
    void SetThrustVector(const FVector& vector) { mThrustVector = vector; }
    void SetThrust(float thrust) { mThrust = thrust; }
    void AutoPilot();

private:
    UPROPERTY();
    TObjectPtr<ABridgeShipPart> Bridge;

    UPROPERTY();
    TArray<TObjectPtr<AShipPart>> ShipParts;

    FVector mThrustVector = FVector::UnitZ();
    FVector mAimPosition = FVector::UnitX();
    float mThrust = 0.0f;
};
