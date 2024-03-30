#include "ShipDesignActions.h"

#include "Ship/ShipDesign.h"

UClass* FShipDesignActions::GetSupportedClass() const
{
    return UShipDesign::StaticClass();
}

FText FShipDesignActions::GetName() const
{
    return INVTEXT("ShipDesign");
}

FColor FShipDesignActions::GetTypeColor() const
{
    return FColor::Cyan;
}

uint32 FShipDesignActions::GetCategories()
{
    return EAssetTypeCategories::Misc;
}
