#include "Ship/AxisComponent.h"
#include "SideViewShootemup/SideViewShootemup.h"
#include "CalcNewPitch.h"


UAxisComponent::UAxisComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAxisComponent::UpdateDirection(const FVector& vector, float time)
{
    double newPitch = CalcNewPitch(GetComponentRotation().Vector(), vector, Traking * time);
    SetWorldRotation(FRotator(newPitch, 0.0f, 0.0f).Quaternion());
}
