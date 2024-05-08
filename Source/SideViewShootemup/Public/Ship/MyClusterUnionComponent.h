#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/ClusterUnionComponent.h"

#include "MyClusterUnionComponent.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API UMyClusterUnionComponent : public UClusterUnionComponent
{
    GENERATED_BODY()
public:
    virtual void AddForceAtLocation(FVector Force, FVector WorldLocation, FName BoneName) override;
};
