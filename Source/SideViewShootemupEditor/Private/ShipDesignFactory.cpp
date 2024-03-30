#include "ShipDesignFactory.h"

#include "Ship/ShipDesign.h"

UShipDesignFactory::UShipDesignFactory()
{
    SupportedClass = UShipDesign::StaticClass();
    bCreateNew = true;
}
UObject* UShipDesignFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    return NewObject<UShipDesign>(InParent, Class, Name, Flags, Context);
}