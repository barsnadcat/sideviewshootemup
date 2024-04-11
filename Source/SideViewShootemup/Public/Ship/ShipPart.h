#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ShipPart.generated.h"

class AShipPawn;
class APlayerController;
class UShipStaticMeshComponent;
class UHealthComponent;
class UBoxComponent;

UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipPart : public AActor
{
    GENERATED_BODY()

public:
    AShipPart();

    UPROPERTY(EditAnywhere)
    TObjectPtr<UBoxComponent> BoxCollisionComponent;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UShipStaticMeshComponent> MainBody;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    TObjectPtr<UBoxComponent> MainBody2;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    TObjectPtr<UHealthComponent> Health;

    virtual void Interact(APlayerController* playerController) {}

    void BreakAndReweldShip();
    void SetCoord(uint8 row, uint8 col)
    {
        Row = row;
        Column = col;
    }

    static void ConnectVertically(AShipPart* top, AShipPart* bottom);
    static void ConnectHorizontally(AShipPart* left, AShipPart* right);

private:
    static int32 Distance(AShipPart* a, AShipPart* b);
    static bool IsConnectedToWeldRoot(AShipPart* part, AShipPart* root, TSet<AShipPart*>& parts);
    static void Reweld(TSet<AShipPart*>& parts);

    void Disconnect();
    AShipPart* GetWeldRoot();

    enum EConnectionIndex : int
    {
        CI_Top = 0,
        CI_Right = 1,
        CI_Bottom = 2,
        CI_Left = 3,
        CI_Size = 4
    };

    TArray<TWeakObjectPtr<AShipPart>> Connections;
    uint8 ConnectedToWeldRoot = 0;
    uint8 Seen = 0;
    uint8 Row = 0;
    uint8 Column = 0;
};
