#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "ShipPart.generated.h"

class AShipPawn;

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

	void Attach(FVector pos, AShipPart* parent, AShipPawn* ship);
protected:
	TWeakObjectPtr<AShipPawn> Ship;
	virtual void OnAttach(AShipPawn* ship) {}
};
