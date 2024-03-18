#pragma once

#include "CoreMinimal.h"
#include "Ship/ShipPart.h"
#include "Components/BoxComponent.h"
#include "BridgeShipPart.generated.h"

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

	virtual void Interact(APlayerController* playerController) override;
};
