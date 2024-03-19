#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"

#include "PhysCharacterMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API UPhysCharacterMovementComponent : public UPawnMovementComponent
{
    GENERATED_BODY()
public:
    virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;
    void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

    UFUNCTION()
    void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UPROPERTY(EditAnywhere)
    double AccelerationScale = 2000.f;

    FVector Acceleration;
    bool IsFalling();

private:
    void UpdatetOrientation(const FVector& v);
    double LastGroundContact = 0.0f;
};
