#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Ship/ShipPawn.h"
#include "ShipAIController.h"
#include "MyGameMode.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API AMyGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
    TSubclassOf<APawn> ShipPawnClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
    TSubclassOf<AShipAIController> ShipAIControllerClass;
    UPROPERTY()
    TObjectPtr<AShipPawn> ShipPawn;
    UPROPERTY()
    TObjectPtr<AShipAIController> ShipAutoPilot;

    virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

    virtual void BeginPlay() override;
};
