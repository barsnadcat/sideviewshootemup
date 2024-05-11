#include "Ship/MyClusterUnionComponent.h"

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

    solver->EnqueueCommandImmediate(
        [solver, particleHandle, force, worldLocation]()
        {
            const Chaos::FVec3 worldCOM = Chaos::FParticleUtilitiesXR::GetCoMWorldPosition(particleHandle);
            const Chaos::FVec3 worldTorque = Chaos::FVec3::CrossProduct(worldLocation - worldCOM, force);
            particleHandle->AddForce(force);
            particleHandle->AddTorque(worldTorque);
            solver->GetEvolution()->WakeParticle(particleHandle);
        });
}