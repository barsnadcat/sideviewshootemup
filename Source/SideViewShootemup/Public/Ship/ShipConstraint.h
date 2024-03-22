#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ShipConstraint.generated.h"

class UPhysicsConstraintComponent;

UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipConstraint : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AShipConstraint();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game);
    TObjectPtr<UPhysicsConstraintComponent> Constraint;
};
