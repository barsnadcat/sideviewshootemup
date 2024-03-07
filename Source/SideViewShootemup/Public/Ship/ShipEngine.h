#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ShipEngine.generated.h"

class AShipPawn;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIDEVIEWSHOOTEMUP_API UShipEngine : public USceneComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UShipEngine();
    void OnConnectToShip(AShipPawn* ship);
    void OnGenerateThrust(UPrimitiveComponent* primitive, const FVector& vector, double thrust, float deltaTime);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    double MaxThrust = 2000000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    double EngineTraking = 180.0f;
};

