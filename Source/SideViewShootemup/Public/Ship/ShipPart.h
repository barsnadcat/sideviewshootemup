#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ShipPart.generated.h"

class AShipPawn;
class APlayerController;
class UGeometryCollectionComponent;
class UHealthComponent;
class UBoxComponent;
class AShipAIController;

UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipPart : public AActor
{
    GENERATED_BODY()

public:
    AShipPart();

    UPROPERTY(EditAnywhere)
    TObjectPtr<UBoxComponent> Overlap;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    TObjectPtr<UHealthComponent> Health;

    TObjectPtr<UPrimitiveComponent> Primitive;
 
    virtual void Interact(APlayerController* playerController) {}
    virtual AShipAIController* GetAutoPilot() { return nullptr; }

    virtual void DisablePart() { }

    void BreakAndReweldShip();
    void SetCoord(uint8 row, uint8 col)
    {
        Row = row;
        Column = col;
    }

    static void ConnectVertically(AShipPart* top, AShipPart* bottom);
    static void ConnectHorizontally(AShipPart* left, AShipPart* right);

protected:
    static int32 Distance(AShipPart* a, AShipPart* b);
    static bool IsConnectedToBridge(AShipPart* part, AShipPart* bridge, TSet<AShipPart*>& parts);
    void Reweld(TSet<AShipPart*>& parts, AShipPawn* pawn);

    void Disconnect();

    enum EConnectionIndex : int
    {
        CI_Top = 0,
        CI_Right = 1,
        CI_Bottom = 2,
        CI_Left = 3,
        CI_Size = 4
    };

    TArray<TWeakObjectPtr<AShipPart>> Connections;
    uint8 ConnectedToBridge = 0;
    uint8 Seen = 0;
    uint8 Row = 0;
    uint8 Column = 0;
};
