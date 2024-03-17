// Copyright 2023 FragLab Ltd. All rights reserved.

#include "Character/PhysCharacterPawn.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/PhysCharacterMovementComponent.h"

APhysCharacterPawn::APhysCharacterPawn(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Structure to hold one-time initialization
    struct FConstructorStatics
    {
        FName ID_Characters;
        FText NAME_Characters;
        FConstructorStatics()
            : ID_Characters(TEXT("Characters"))
            , NAME_Characters(NSLOCTEXT("SpriteCategory", "Characters", "Characters"))
        {
        }
    };
    static FConstructorStatics ConstructorStatics;

    // Character rotation only changes in Yaw, to prevent the capsule from changing orientation.
    // Ask the Controller for the full rotation if desired (ie for aiming).
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = true;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
    CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

    CapsuleComponent->CanCharacterStepUpOn = ECB_No;
    CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
    CapsuleComponent->SetCanEverAffectNavigation(false);
    CapsuleComponent->bDynamicObstacle = true;
    RootComponent = CapsuleComponent;

    PhysCharacterMovement = CreateDefaultSubobject<UPhysCharacterMovementComponent>(TEXT("MovementComponent"));
    if (PhysCharacterMovement)
    {
        PhysCharacterMovement->UpdatedComponent = CapsuleComponent;
    }

#if WITH_EDITORONLY_DATA
    ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
    if (ArrowComponent)
    {
        ArrowComponent->ArrowColor = FColor(150, 200, 255);
        ArrowComponent->bTreatAsASprite = true;
        ArrowComponent->SpriteInfo.Category = ConstructorStatics.ID_Characters;
        ArrowComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Characters;
        ArrowComponent->SetupAttachment(CapsuleComponent);
        ArrowComponent->bIsScreenSizeScaled = true;
        ArrowComponent->SetSimulatePhysics(false);
    }
#endif // WITH_EDITORONLY_DATA

    Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    if (Mesh)
    {
        Mesh->AlwaysLoadOnClient = true;
        Mesh->AlwaysLoadOnServer = true;
        Mesh->bOwnerNoSee = false;
        Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
        Mesh->bCastDynamicShadow = true;
        Mesh->bAffectDynamicIndirectLighting = true;
        Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
        Mesh->SetupAttachment(CapsuleComponent);
        static FName MeshCollisionProfileName(TEXT("CharacterMesh"));
        Mesh->SetCollisionProfileName(MeshCollisionProfileName);
        Mesh->SetGenerateOverlapEvents(false);
        Mesh->SetCanEverAffectNavigation(false);
    }
}

FVector APhysCharacterPawn::GetAcceleration() const
{
    return PhysCharacterMovement ? PhysCharacterMovement->Acceleration : FVector::Zero();
}

bool APhysCharacterPawn::IsFalling() const
{
    return PhysCharacterMovement ? PhysCharacterMovement->IsFalling() : false;
}

// Called when the game starts or when spawned
void APhysCharacterPawn::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void APhysCharacterPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APhysCharacterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
