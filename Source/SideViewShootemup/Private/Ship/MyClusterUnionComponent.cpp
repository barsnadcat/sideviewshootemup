#include "Ship/MyClusterUnionComponent.h"

#include "PhysicsProxy/ClusterUnionPhysicsProxy.h"

void UMyClusterUnionComponent::AddForceAtLocation(FVector Force, FVector WorldLocation, FName BoneName)
{
    if (auto* proxy = GetPhysicsProxy())
    {
        if (Chaos::FPhysicsSolver* RBDSolver = proxy->GetSolver<Chaos::FPhysicsSolver>())
        {
            WakeAllRigidBodies();
            RBDSolver->EnqueueCommandImmediate([proxy, Force, WorldLocation, RBDSolver]()
                {
				Chaos::FReal ClosestDistanceSquared = TNumericLimits<Chaos::FReal>::Max();
				Chaos::FPBDRigidClusteredParticleHandle* ClosestHandle = nullptr;

				Chaos::FRigidClustering& Clustering = RBDSolver->GetEvolution()->GetRigidClustering();
				for (Chaos::FPBDRigidClusteredParticleHandle* ClusteredHandle : Clustering.GetTopLevelClusterParents())
				{
                    if (ClusteredHandle && ClusteredHandle->PhysicsProxy() == proxy)
					{
						if (ClusteredHandle->IsDynamic())
						{
							const Chaos::FReal DistanceSquared = (WorldLocation - ClusteredHandle->GetX()).SquaredLength();
							if (DistanceSquared < ClosestDistanceSquared)
							{
								ClosestDistanceSquared = DistanceSquared;
								ClosestHandle = ClusteredHandle;
							}
						}
					}
				}
				if (ClosestHandle)
				{
					const Chaos::FVec3 WorldCOM = Chaos::FParticleUtilitiesXR::GetCoMWorldPosition(ClosestHandle);
					const Chaos::FVec3 WorldTorque = Chaos::FVec3::CrossProduct(WorldLocation - WorldCOM, Force);
					ClosestHandle->AddForce(Force);
					ClosestHandle->AddTorque(WorldTorque);
				} });
        }
    }
}