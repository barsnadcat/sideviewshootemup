#pragma once

#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"

#include "ShipStaticMeshComponent.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API UShipStaticMeshComponent : public UStaticMeshComponent
{
    GENERATED_BODY()
public:
    virtual void UnWeldFromParent() override;
};
