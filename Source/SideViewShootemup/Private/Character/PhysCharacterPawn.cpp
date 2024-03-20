#include "Character/PhysCharacterPawn.h"

#include "Character/PhysCharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Ship/ShipPart.h"
#include "SideViewShootemup/SideViewShootemup.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

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

    CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &APhysCharacterPawn::OnBeginOverlap);
    CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &APhysCharacterPawn::OnEndOverlap);

    RootComponent = CapsuleComponent;

    PhysCharacterMovement = CreateDefaultSubobject<UPhysCharacterMovementComponent>(TEXT("MovementComponent"));
    if (PhysCharacterMovement)
    {
        PhysCharacterMovement->UpdatedComponent = CapsuleComponent;
    }

    Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
    Constraint->SetupAttachment(CapsuleComponent);

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
#endif    // WITH_EDITORONLY_DATA

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

void APhysCharacterPawn::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(Game, Display, TEXT("Begin: comp:%s other:%s othercomp:%s bodyIndex:%d fromSweep:%d result:%d"),
        OverlappedComp ? *OverlappedComp->GetFName().ToString() : TEXT("null"),
        Other ? *Other->GetFName().ToString() : TEXT("null"),
        OtherComp ? *OtherComp->GetFName().ToString() : TEXT("null"), OtherBodyIndex, bFromSweep, SweepResult.bBlockingHit);

    if (AShipPart* part = Cast<AShipPart>(Other))
    {
        OverlappedShipPart = part;
    }
}

void APhysCharacterPawn::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UE_LOG(Game, Display, TEXT("End: comp:%s other:%s othercomp:%s bodyIndex:%d"),
        OverlappedComp ? *OverlappedComp->GetFName().ToString() : TEXT("null"),
        Other ? *Other->GetFName().ToString() : TEXT("null"),
        OtherComp ? *OtherComp->GetFName().ToString() : TEXT("null"), OtherBodyIndex);
    if (AShipPart* part = Cast<AShipPart>(Other))
    {
        if (part == OverlappedShipPart)
        {
            OverlappedShipPart = nullptr;
        }
    }
}

FVector APhysCharacterPawn::GetAcceleration() const
{
    return PhysCharacterMovement ? PhysCharacterMovement->GetAcceleration() : FVector::ZeroVector;
}

bool APhysCharacterPawn::IsFalling() const
{
    return PhysCharacterMovement ? PhysCharacterMovement->IsFalling() : false;
}

void APhysCharacterPawn::Interact(APlayerController* playerController)
{
    if (AShipPart* part = OverlappedShipPart.Get())
    {
        part->Interact(playerController);
    }
}

void APhysCharacterPawn::Attach(AShipPart* part)
{
    Constraint->ConstraintActor1 = this;
    Constraint->ConstraintActor2 = part;
    Constraint->InitComponentConstraint();
}

void APhysCharacterPawn::Detach()
{
    Constraint->BreakConstraint();
}
