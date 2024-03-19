#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "BTTaskNode_Hover.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API UBTTaskNode_Hover : public UBTTaskNode
{
    GENERATED_BODY()

    UBTTaskNode_Hover(const FObjectInitializer& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
