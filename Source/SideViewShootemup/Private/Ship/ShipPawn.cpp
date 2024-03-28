#include "Ship/ShipPawn.h"

#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Ship/BridgeShipPart.h"
#include "Ship/ShipPart.h"
#include "ShipAIController.h"
#include "SideViewShootemup/SideViewShootemup.h"

AShipPawn::AShipPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AShipPawn::BeginPlay()
{
    Super::BeginPlay();
    UWorld* world = GetWorld();
    if (!world || !BridgeClass || !SidePartClass || !TopPartClass)
    {
        return;
    }

    FActorSpawnParameters spawnParams;
    spawnParams.bNoFail = true;
    spawnParams.Owner = this;
    Bridge = world->SpawnActor<ABridgeShipPart>(BridgeClass, GetActorTransform(), spawnParams);

    RootComponent->AttachToComponent(Bridge->MainBody, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    AShipPart* top = world->SpawnActor<AShipPart>(TopPartClass, Bridge->GetActorLocation() + FVector(0.f, 0.f, 220.0f), Bridge->GetActorRotation(), spawnParams);
    AShipPart::Weld(Bridge, top);

    AShipPart* left = world->SpawnActor<AShipPart>(SidePartClass, Bridge->GetActorLocation() + FVector(-220.0f, 0.f, 0.f), Bridge->GetActorRotation(), spawnParams);
    AShipPart::Weld(Bridge, left);

    AShipPart* right = world->SpawnActor<AShipPart>(SidePartClass, Bridge->GetActorLocation() + FVector(220.0f, 0.f, 0.f), Bridge->GetActorRotation(), spawnParams);
    AShipPart::Weld(Bridge, right);

    AutoPilot();
}


void AShipPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateThrust.Broadcast(mThrustVector, mThrust, DeltaTime);
    UpdateAimTarget.Broadcast(mAimPosition, DeltaTime);

    mThrust = 0.0f;
    mThrustVector = FVector::UnitZ();
}

void AShipPawn::AutoPilot()
{
    if (IsValid(Bridge))
    {
        Bridge->GetAutoPilot()->Possess(this);
    }
}
