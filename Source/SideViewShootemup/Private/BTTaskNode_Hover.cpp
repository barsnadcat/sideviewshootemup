// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_Hover.h"
#include "SideViewShootemup\SideViewShootemup.h"

UBTTaskNode_Hover::UBTTaskNode_Hover(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTaskNode_Hover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UE_LOG(Game, Display, TEXT("ExecuteTask"));
    return EBTNodeResult::InProgress;
}

void UBTTaskNode_Hover::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UE_LOG(Game, Display, TEXT("TickTask"));
}
