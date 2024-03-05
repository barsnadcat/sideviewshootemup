#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShipPart.generated.h"

UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipPart : public AActor
{
	GENERATED_BODY()
	
public:	
	AShipPart();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
	TObjectPtr<UStaticMeshComponent> MainBody;

};
