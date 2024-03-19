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

    virtual bool IsFalling() const override;
    const FVector& GetAcceleration() const { return Acceleration; }

private:
    void UpdatetOrientation(const FVector& v);
    FVector Acceleration;
    double LastGroundContact = 0.0f;
};
