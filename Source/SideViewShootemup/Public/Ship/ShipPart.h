#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ShipPart.generated.h"

class AShipPawn;
class APlayerController;
class UPhysicsConstraintComponent;
class UHealthComponent;

UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipPart : public AActor
{
    GENERATED_BODY()

public:
    AShipPart();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    TObjectPtr<UStaticMeshComponent> MainBody;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    TObjectPtr<UHealthComponent> Health;

    virtual void Interact(APlayerController* playerController) {}

    virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

    static void Weld(AShipPart* a, AShipPart* b);

private:
    void AddWeld(AShipPart* otherShip, TSharedPtr<FConstraintInstance> weld);
    void RemoveWeld(AShipPart* otherShipPart);

    TMap<AShipPart*, TSharedPtr<FConstraintInstance>> welds;
};
