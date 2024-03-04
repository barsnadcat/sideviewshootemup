#include "BTTaskNode_Hover.h"
#include "AIController.h"
#include "ShipPawn.h"
#include "SideViewShootemup\SideViewShootemup.h"

UBTTaskNode_Hover::UBTTaskNode_Hover(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTaskNode_Hover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::InProgress;
}

void UBTTaskNode_Hover::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* controller = OwnerComp.GetAIOwner();
    if (controller)
    {
        AShipPawn* ship = controller->GetPawn<AShipPawn>();
        if (ship)
        {
            ship->SetThrustVector({ 0.0f, 0.0f, 1.f });
            ship->SetThrust(0.25f);
        }
    }
}
