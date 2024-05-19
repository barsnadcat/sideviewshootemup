#include "Ship/MyClusterUnionComponent.h"

#include "PhysicsEngine/ConstraintInstance.h"
#include "PhysicsEngine/PhysicsObjectExternalInterface.h"
#include "PhysicsProxy/ClusterUnionPhysicsProxy.h"

void UMyClusterUnionComponent::AddForceAtLocation(FVector force, FVector worldLocation, FName boneName)
{
    auto* proxy = GetPhysicsProxy();
    if (!proxy)
    {
        return;
    }
    Chaos::FPhysicsSolver* solver = proxy->GetSolver<Chaos::FPhysicsSolver>();
    Chaos::FPBDRigidClusteredParticleHandle* particleHandle = proxy->GetParticle_Internal();
    if (!particleHandle || !solver)
    {
        return;
    }
    float etherLinearDrag = 1.5f; 
    float etherAngularDrag = 1.5f;

    solver->EnqueueCommandImmediate(
        [solver, particleHandle, force, worldLocation, etherLinearDrag, etherAngularDrag]()
        {
            const Chaos::FVec3 worldCOM = Chaos::FParticleUtilitiesXR::GetCoMWorldPosition(particleHandle);
            const Chaos::FVec3 worldTorque = Chaos::FVec3::CrossProduct(worldLocation - worldCOM, force);
            particleHandle->AddForce(force);
            particleHandle->AddTorque(worldTorque);
            particleHandle->SetLinearEtherDrag(etherLinearDrag);
            particleHandle->SetAngularEtherDrag(etherAngularDrag);
            solver->GetEvolution()->WakeParticle(particleHandle);
        });
}

void UMyClusterUnionComponent::OnCreatePhysicsState()
{
    Super::OnCreatePhysicsState();
    CreateDOFLock();
}

FTransform UMyClusterUnionComponent::GetUnrealWorldTransform() const
{
    if (Chaos::FClusterUnionPhysicsProxy* proxy = GetPhysicsProxy())
    {
        if (FChaosScene* scene = FPhysicsObjectExternalInterface::GetScene(proxy->GetPhysicsObjectHandle()))
        {
            FScopedSceneLock_Chaos SceneLock(scene, EPhysicsInterfaceScopedLockType::Read);
            return Chaos::FRigidTransform3(proxy->GetParticle_External()->X(), proxy->GetParticle_External()->R());
        }
    }
    return FTransform::Identity;
}

void UMyClusterUnionComponent::CreateDOFLock()
{
    auto* proxy = GetPhysicsProxy();
    if (!proxy)
    {
        return;
    }

    if (DOFConstraint)
    {
        DOFConstraint->TermConstraint();
        FConstraintInstance::Free(DOFConstraint);
        DOFConstraint = NULL;
    }


    DOFConstraint = FConstraintInstance::Alloc();

    DOFConstraint->ProfileInstance.ConeLimit.bSoftConstraint = false;
    DOFConstraint->ProfileInstance.TwistLimit.bSoftConstraint = false;
    DOFConstraint->ProfileInstance.LinearLimit.bSoftConstraint = false;


    DOFConstraint->SetAngularSwing1Motion(EAngularConstraintMotion::ACM_Locked);
    DOFConstraint->SetAngularSwing2Motion(EAngularConstraintMotion::ACM_Locked);
    DOFConstraint->SetAngularTwistMotion(EAngularConstraintMotion::ACM_Free);

    DOFConstraint->SetLinearXMotion(ELinearConstraintMotion::LCM_Locked);
    DOFConstraint->SetLinearYMotion(ELinearConstraintMotion::LCM_Free);
    DOFConstraint->SetLinearZMotion(ELinearConstraintMotion::LCM_Free);

    const FVector LockedAxis = FVector(0, 1, 0);
    FVector Normal = LockedAxis.GetSafeNormal();
    FVector Garbage;
    FVector Sec = FVector(0, 1, 0);
    Normal.FindBestAxisVectors(Garbage, Sec);

    const FTransform TM = GetUnrealWorldTransform();

    DOFConstraint->PriAxis1 = TM.InverseTransformVectorNoScale(Normal);
    DOFConstraint->SecAxis1 = TM.InverseTransformVectorNoScale(Sec);

    DOFConstraint->PriAxis2 = Normal;
    DOFConstraint->SecAxis2 = Sec;
    DOFConstraint->Pos2 = TM.GetLocation();

    // Create constraint instance based on DOF
    DOFConstraint->InitConstraint(proxy->GetPhysicsObjectHandle(), nullptr, 1.f, this);
}