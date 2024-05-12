#pragma once

#include "CoreMinimal.h"
#include "Ship/AxisComponent.h"
#include "Ship/ShipPart.h"

#include "EngineShipPart.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API AEngineShipPart : public AShipPart
{
    GENERATED_BODY()
public:
    AEngineShipPart();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    double MaxThrust = 2000000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    TObjectPtr<UAxisComponent> Axis;

private:
    virtual void DisablePart() override;
    virtual void PostActorCreated() override;
    void OnUpdateThrust(const FVector& vector, double thrust, float deltaTime);
    FDelegateHandle OnUpdateThrustHandle;
};
