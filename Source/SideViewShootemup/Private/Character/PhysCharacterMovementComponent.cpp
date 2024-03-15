#include "Character/PhysCharacterMovementComponent.h"
#include "SideViewShootemup/SideViewShootemup.h"

void UPhysCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    FVector InputVector = ConsumeInputVector();

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!UpdatedPrimitive)
    {
        return;
    }
    Acceleration = InputVector * AccelerationScale;
    
    if (!Acceleration.IsNearlyZero())
    {
        UpdatedPrimitive->AddForce(Acceleration, NAME_None, true);

        FRotator orientation;
        if (Acceleration.X > 0.01f)
        {
            orientation.Yaw = 0.0f;
            MoveUpdatedComponent(FVector::ZeroVector, orientation, true);
        }
        else if (Acceleration.X < -0.01f)
        {
            orientation.Yaw = 180.0f;
            MoveUpdatedComponent(FVector::ZeroVector, orientation, true);
        }
    }

}
