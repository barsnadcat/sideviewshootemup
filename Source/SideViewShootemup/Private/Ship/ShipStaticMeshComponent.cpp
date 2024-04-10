#include "Ship/ShipStaticMeshComponent.h"

namespace
{
UPrimitiveComponent* GetRootWelded(const UPrimitiveComponent* PrimComponent, FName ParentSocketName = NAME_None, FName* OutSocketName = NULL, bool bAboutToWeld = false)
{
    UPrimitiveComponent* Result = NULL;
    UPrimitiveComponent* RootComponent = Cast<UPrimitiveComponent>(PrimComponent->GetAttachParent());    // we must find the root component along hierarchy that has bWelded set to true

    // check that body itself is welded
    if (FBodyInstance* BI = PrimComponent->GetBodyInstance(ParentSocketName, false))
    {
        if (bAboutToWeld == false && BI->WeldParent == nullptr && BI->bAutoWeld == false)    // we're not welded and we aren't trying to become welded
        {
            return NULL;
        }
    }

    FName PrevSocketName = ParentSocketName;
    FName SocketName = NAME_None;    // because of skeletal mesh it's important that we check along the bones that we attached
    FBodyInstance* RootBI = NULL;
    for (; RootComponent; RootComponent = Cast<UPrimitiveComponent>(RootComponent->GetAttachParent()))
    {
        Result = RootComponent;
        SocketName = PrevSocketName;
        PrevSocketName = RootComponent->GetAttachSocketName();

        RootBI = RootComponent->GetBodyInstance(SocketName, false);
        if (RootBI && RootBI->WeldParent != nullptr)
        {
            continue;
        }

        break;
    }

    if (OutSocketName)
    {
        *OutSocketName = SocketName;
    }

    return Result;
}

void UnWeld2(FBodyInstance* MyBI, FBodyInstance* TheirBI)
{
    // check(IsInGameThread());

    bool bShapesChanged = false;

    FPhysicsCommand::ExecuteWrite(MyBI->ActorHandle,
        [&MyBI, &TheirBI, &bShapesChanged](const FPhysicsActorHandle& Actor)
        {
            TArray<FPhysicsShapeHandle> Shapes;
            const int32 NumSyncShapes = MyBI->GetAllShapes_AssumesLocked(Shapes);
            const int32 NumTotalShapes = Shapes.Num();

            // reversed since FPhysicsInterface::DetachShape is removing shapes
                for (int Idx = Shapes.Num() - 1; Idx >= 0; Idx--)
                {
                    FPhysicsShapeHandle& Shape = Shapes[Idx];
                    const FBodyInstance* BI = MyBI->GetOriginalBodyInstance(Shape);
                    if (TheirBI == BI)
                    {
                        const_cast<TMap<FPhysicsShapeHandle, FBodyInstance::FWeldInfo>*>(MyBI->GetCurrentWeldInfo())->Remove(Shape);
                        {
                            SCOPED_NAMED_EVENT_TEXT("FPhysicsInterface::DetachShape", FColor::Red);
                            FPhysicsInterface::DetachShape(Actor, Shape);
                        }
                        bShapesChanged = true;
                    }
                }

            if (bShapesChanged)
            {
                SCOPED_NAMED_EVENT_TEXT("MyBI->PostShapeChange", FColor::Red);
                MyBI->PostShapeChange();
            }

            TheirBI->WeldParent = nullptr;
        });

    // MyBI->UpdateInterpolateWhenSubStepping();

    // TheirBI->UpdateDebugRendering();
    // MyBI->UpdateDebugRendering();
}

}    // namespace

void UShipStaticMeshComponent::UnWeldFromParent()
{
    SCOPED_NAMED_EVENT_TEXT("UnWeldFromParent", FColor::Red);

    FBodyInstance* NewRootBI = GetBodyInstance(NAME_None, false);
    UWorld* CurrentWorld = GetWorld();
    if (NewRootBI == NULL || NewRootBI->WeldParent == nullptr || CurrentWorld == nullptr || CurrentWorld->GetPhysicsScene() == nullptr || !IsValidChecked(this) || IsUnreachable())
    {
        return;
    }

    // If we're purging (shutting down everything to kill the runtime) don't proceed
    // to make new physics bodies and weld them, as they'll never be used.
    if (GExitPurge)
    {
        return;
    }

    FName SocketName;
    UPrimitiveComponent* RootComponent = GetRootWelded(this, GetAttachSocketName(), &SocketName);

    if (RootComponent)
    {
        if (FBodyInstance* RootBI = RootComponent->GetBodyInstance(SocketName, false))
        {
            bool bRootIsBeingDeleted = !IsValidChecked(RootComponent) || RootComponent->IsUnreachable();
            const FBodyInstance* PrevWeldParent = NewRootBI->WeldParent;
            {
                UnWeld2(RootBI, NewRootBI);
                // RootBI->UnWeld(NewRootBI);
            }

            FPlatformAtomics::InterlockedExchangePtr((void**) &NewRootBI->WeldParent, nullptr);

            bool bHasBodySetup = GetBodySetup() != nullptr;

            // if BodyInstance hasn't already been created we need to initialize it
            if (!bRootIsBeingDeleted && bHasBodySetup && NewRootBI->IsValidBodyInstance() == false)
            {
                bool bPrevAutoWeld = NewRootBI->bAutoWeld;
                NewRootBI->bAutoWeld = false;
                NewRootBI->InitBody(GetBodySetup(), GetComponentToWorld(), this, CurrentWorld->GetPhysicsScene());
                NewRootBI->bAutoWeld = bPrevAutoWeld;
            }

            if (PrevWeldParent == nullptr)    // our parent is kinematic so no need to do any unwelding/rewelding of children
            {
                return;
            }

            // now weld its children to it
            TArray<FBodyInstance*> ChildrenBodies;
            TArray<FName> ChildrenLabels;
            GetWeldedBodies(ChildrenBodies, ChildrenLabels);

            for (int32 ChildIdx = 0; ChildIdx < ChildrenBodies.Num(); ++ChildIdx)
            {
                FBodyInstance* ChildBI = ChildrenBodies[ChildIdx];
                checkSlow(ChildBI);
                if (ChildBI != NewRootBI)
                {
                    if (!bRootIsBeingDeleted)
                    {
                        RootBI->UnWeld(ChildBI);
                    }

                    // At this point, NewRootBI must be kinematic because it's being unwelded.
                    FPlatformAtomics::InterlockedExchangePtr((void**) &ChildBI->WeldParent, nullptr);    // null because we are currently kinematic
                }
            }

            // If the new root body is simulating, we need to apply the weld on the children
            if (!bRootIsBeingDeleted && NewRootBI->IsInstanceSimulatingPhysics())
            {
                NewRootBI->ApplyWeldOnChildren();
            }
        }
    }
}
