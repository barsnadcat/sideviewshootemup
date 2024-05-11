#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/ClusterUnionComponent.h"

#include "MyClusterUnionComponent.generated.h"

struct FConstraintInstance;
/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API UMyClusterUnionComponent : public UClusterUnionComponent
{
    GENERATED_BODY()
public:
    virtual void AddForceAtLocation(FVector Force, FVector WorldLocation, FName BoneName) override;
    virtual void OnCreatePhysicsState() override;

private:
    FTransform GetUnrealWorldTransform() const;
    void CreateDOFLock();

private:
    FConstraintInstance* DOFConstraint = nullptr;

};
