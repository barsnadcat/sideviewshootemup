#pragma once

#include "CoreMinimal.h"
#include "Ship/ShipPart.h"
#include "Components/BoxComponent.h"
#include "BridgeShipPart.generated.h"

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

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollisionComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<AShipAIController> ShipAIControllerClass;

	UPROPERTY()
	TObjectPtr<AShipAIController> ShipAutoPilot;

	virtual void BeginPlay();

	virtual void Interact(APlayerController* playerController) override;

};
