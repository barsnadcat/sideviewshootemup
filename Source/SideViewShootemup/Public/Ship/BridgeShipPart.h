#pragma once

#include "CoreMinimal.h"
#include "Ship/ShipPart.h"

#include "BridgeShipPart.generated.h"

class UPhysicsConstraintComponent;

class AShipAIController;
/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API ABridgeShipPart : public AShipPart
{
    GENERATED_BODY()
public:
    ABridgeShipPart();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
    TSubclassOf<AShipAIController> ShipAIControllerClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game);
    TObjectPtr<UPhysicsConstraintComponent> PawnConstraint;

    virtual void Interact(APlayerController* playerController) override;
    AShipAIController* GetAutoPilot() { return ShipAutoPilot; }

private:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

    TObjectPtr<AShipAIController> ShipAutoPilot;
};
