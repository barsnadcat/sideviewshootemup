#pragma once

#include "AssetTypeActions_Base.h"
#include "CoreMinimal.h"

/**
 *
 */
class FShipDesignActions : public FAssetTypeActions_Base
{
public:
    UClass* GetSupportedClass() const override;
    FText GetName() const override;
    FColor GetTypeColor() const override;
    uint32 GetCategories() override;
};
