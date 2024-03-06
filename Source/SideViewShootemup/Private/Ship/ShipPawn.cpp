#include "Ship/ShipPawn.h"
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
    Bridge = world->SpawnActor<ABridgeShipPart>(BridgeClass, spawnParams);

    Bridge->SetActorTransform(GetActorTransform());
    SetActorTransform(FTransform::Identity);
    RootComponent->AttachToComponent(Bridge->MainBody, FAttachmentTransformRules::KeepRelativeTransform);

    AShipPart* top = world->SpawnActor<AShipPart>(TopPartClass, spawnParams);
    top->Attach(FVector(0.f, 0.f, 110.0f), Bridge);
    ShipParts.Add(top);

    AShipPart* left = world->SpawnActor<AShipPart>(SidePartClass, spawnParams);
    left->Attach(FVector(-110.0f, 0.f, 0.f), Bridge);
    ShipParts.Add(left);

    AShipPart* right = world->SpawnActor<AShipPart>(SidePartClass, spawnParams);
    right->Attach(FVector(110.0f, 0.f, 0.f), Bridge);
    ShipParts.Add(right);
}

void AShipPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    GenerateThrust.Broadcast(Bridge->MainBody, mThrustVector, mThrust, DeltaTime);
    AimAt.Broadcast(mAimPosition, DeltaTime);

    mThrust = 0.0f;
    mThrustVector = FVector::UnitZ();
}
