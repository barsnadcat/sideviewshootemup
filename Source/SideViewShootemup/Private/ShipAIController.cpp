#include "ShipAIController.h"

void AShipAIController::OnPossess(APawn* inPawn)
{
    Super::OnPossess(inPawn);
    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
    }
}
