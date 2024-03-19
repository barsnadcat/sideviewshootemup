#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "ShipPart.generated.h"

class AShipPawn;
class APlayerController;

UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipPart : public AActor
{
    GENERATED_BODY()

public:
    AShipPart();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    TObjectPtr<UStaticMeshComponent> MainBody;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game);
    TObjectPtr<UPhysicsConstraintComponent> Constraint;

    void Attach(AShipPart* parent);
    virtual void Interact(APlayerController* playerController) {}
    void SetShip(AShipPawn* ship);

protected:
    TWeakObjectPtr<AShipPawn> Ship;
    virtual void OnSetShip(AShipPawn* ship) {}
};
