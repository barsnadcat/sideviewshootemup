#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "ShipDesign.generated.h"

class ABridgeShipPart;
class AShipPart;

USTRUCT()
struct FShipPartArray
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<AShipPart>> Y;
};

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API UShipDesign : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
    double GridSize = 220.0f;

    UPROPERTY(EditAnywhere)
    TArray<FShipPartArray> X;
};
