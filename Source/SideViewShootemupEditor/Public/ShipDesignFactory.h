#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"

#include "ShipDesignFactory.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUPEDITOR_API UShipDesignFactory : public UFactory
{
    GENERATED_BODY()
public:
    UShipDesignFactory();
    UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn);
};