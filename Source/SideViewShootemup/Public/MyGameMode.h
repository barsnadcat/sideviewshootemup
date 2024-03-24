#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "MyGameMode.generated.h"

class AShipPawn;
class AShipAIController;

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

    virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
    virtual void BeginPlay() override;

private:
    TObjectPtr<AShipPawn> ShipPawn;
};
