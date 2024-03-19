#pragma once

#include "AIController.h"
#include "CoreMinimal.h"

#include "ShipAIController.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API AShipAIController : public AAIController
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = AI)
    TObjectPtr<UBehaviorTree> BehaviorTree;

    virtual void OnPossess(APawn* InPawn) override;
};
