#include "Ship/ShipPart.h"

#include "HealthComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Ship/ShipPawn.h"
#include "SideViewShootemup/SideViewShootemup.h"

AShipPart::AShipPart()
{
    PrimaryActorTick.bCanEverTick = false;
    MainBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainBody"));
    RootComponent = MainBody;
    Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

void AShipPart::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    Super::EndPlay(endPlayReason);

    for (const TPair<TObjectPtr<AShipPart>, TSharedPtr<FConstraintInstance>>& pair : welds)
    {
        if (pair.Key)
        {
            pair.Key->RemoveWeld(this);
        }
        if (pair.Value)
        {
            pair.Value->TermConstraint();
        }
    }
    welds.Empty();
}

void AShipPart::AddWeld(AShipPart* otherShipPart, TSharedPtr<FConstraintInstance> weld)
{
    check(otherShipPart);
    welds.Add(otherShipPart, weld);
}

void AShipPart::RemoveWeld(AShipPart* otherShipPart)
{
    check(otherShipPart);
    welds.Remove(otherShipPart);
}

void AShipPart::Weld(AShipPart* p1, AShipPart* p2)
{
    if (!p1 || !p2)
    {
        return;
    }
    check(p1 != p2);

    TSharedPtr<FConstraintInstance> weld = MakeShared<FConstraintInstance>();

    weld->ProfileInstance.bDisableCollision = true;
    weld->ProfileInstance.bEnableMassConditioning = false;
    weld->ProfileInstance.bEnableShockPropagation = false;
    weld->ProfileInstance.bLinearBreakable = false;
    weld->ProfileInstance.bLinearPlasticity = false;
    weld->ProfileInstance.bAngularBreakable = false;
    weld->ProfileInstance.bAngularPlasticity = false;

    weld->ProfileInstance.bEnableProjection = true;
    weld->ProfileInstance.ProjectionLinearTolerance = 0.1f;
    weld->ProfileInstance.ProjectionLinearAlpha = 0.0f;
    weld->ProfileInstance.ProjectionAngularTolerance = 0.1f;
    weld->ProfileInstance.ProjectionAngularAlpha = 0.0f;

    weld->ProfileInstance.ContactTransferScale = 1.0f;

    weld->ProfileInstance.LinearLimit.bSoftConstraint = false;
    weld->ProfileInstance.LinearLimit.Limit = 0.0f;
    weld->ProfileInstance.LinearLimit.ContactDistance = 50.0f;
    weld->ProfileInstance.LinearLimit.Restitution = 1.0f;
    weld->ProfileInstance.LinearLimit.XMotion = ELinearConstraintMotion::LCM_Locked;
    weld->ProfileInstance.LinearLimit.YMotion = ELinearConstraintMotion::LCM_Locked;
    weld->ProfileInstance.LinearLimit.ZMotion = ELinearConstraintMotion::LCM_Locked;

    weld->ProfileInstance.ConeLimit.bSoftConstraint = false;
    weld->ProfileInstance.ConeLimit.Swing1Motion = EAngularConstraintMotion::ACM_Locked;
    weld->ProfileInstance.ConeLimit.Swing2Motion = EAngularConstraintMotion::ACM_Locked;

    weld->ProfileInstance.TwistLimit.bSoftConstraint = false;
    weld->ProfileInstance.TwistLimit.TwistMotion = EAngularConstraintMotion::ACM_Locked;

    const FVector pos = (p1->GetActorLocation() - p2->GetActorLocation()) * 0.5f + p2->GetActorLocation();
    // First we convert world space position of constraint into local space frames
    FTransform t1 = p1->MainBody->GetComponentTransform();
    t1.RemoveScaling();
    FTransform t2 = p2->MainBody->GetComponentTransform();
    t2.RemoveScaling();
    weld->Pos1 = t1.InverseTransformPosition(pos);
    weld->Pos2 = t2.InverseTransformPosition(pos);

    weld->InitConstraint(p1->MainBody->GetPhysicsObjectById(0), p2->MainBody->GetPhysicsObjectById(0), 1.0f, nullptr);

    p1->AddWeld(p2, weld);
    p2->AddWeld(p1, weld);
}
