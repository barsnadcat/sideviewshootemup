#include "Ship/ShipPawn.h"

#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Ship/BridgeShipPart.h"
#include "Ship/ShipConstraint.h"
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
    Attach(Bridge, top);

    AShipPart* left = world->SpawnActor<AShipPart>(SidePartClass, Bridge->GetActorLocation() + FVector(-220.0f, 0.f, 0.f), Bridge->GetActorRotation(), spawnParams);
    Attach(Bridge, left);

    AShipPart* right = world->SpawnActor<AShipPart>(SidePartClass, Bridge->GetActorLocation() + FVector(220.0f, 0.f, 0.f), Bridge->GetActorRotation(), spawnParams);
    Attach(Bridge, right);

    AutoPilot();
}

void AShipPawn::Attach(AShipPart* parent, AShipPart* child)
{
    UWorld* world = GetWorld();
    check(world);
    FVector jointPos = (parent->GetActorLocation() - child->GetActorLocation()) * 0.5f + child->GetActorLocation();
    FActorSpawnParameters spawnParams;
    spawnParams.bNoFail = true;
    spawnParams.Owner = this;
    AShipConstraint* constraint = world->SpawnActor<AShipConstraint>(ConstraintClass, jointPos, FRotator::ZeroRotator, spawnParams);
    check(constraint);
    constraint->Constraint->ConstraintActor1 = child;
    constraint->Constraint->ConstraintActor2 = parent;
    constraint->Constraint->InitComponentConstraint();
    // UE_LOG(Game, Display, TEXT("%s %s %.2f"), *child->GetFName().ToString(), *parent->GetFName().ToString(), constraint->Constraint->ConstraintInstance.AverageMass);
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
