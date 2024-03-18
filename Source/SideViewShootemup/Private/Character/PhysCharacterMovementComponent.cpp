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
        UpdatetOrientation(Acceleration);
    }
}

void UPhysCharacterMovementComponent::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
    Super::SetUpdatedComponent(NewUpdatedComponent);
    if (UpdatedPrimitive)
    {
        UpdatedPrimitive->OnComponentBeginOverlap.AddDynamic(this, &UPhysCharacterMovementComponent::OnBeginOverlap);
        UpdatedPrimitive->OnComponentEndOverlap.AddDynamic(this, &UPhysCharacterMovementComponent::OnEndOverlap);
        UpdatedPrimitive->OnComponentHit.AddDynamic(this, &UPhysCharacterMovementComponent::OnComponentHit);
    }
}

void UPhysCharacterMovementComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(Game, Display, TEXT("Begin: comp:%s other:%s othercomp:%s bodyIndex:%d fromSweep:%d result:%d"),
        OverlappedComp ? *OverlappedComp->GetFName().ToString() : TEXT("null"),
        Other ? *Other->GetFName().ToString() : TEXT("null"),
        OtherComp ? *OtherComp->GetFName().ToString() : TEXT("null"), OtherBodyIndex, bFromSweep, SweepResult.bBlockingHit);
    
}

void UPhysCharacterMovementComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UE_LOG(Game, Display, TEXT("End: comp:%s other:%s othercomp:%s bodyIndex:%d"),
        OverlappedComp ? *OverlappedComp->GetFName().ToString() : TEXT("null"),
        Other ? *Other->GetFName().ToString() : TEXT("null"),
        OtherComp ? *OtherComp->GetFName().ToString() : TEXT("null"), OtherBodyIndex);
}

void UPhysCharacterMovementComponent::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    /*
    UE_LOG(Game, Display, TEXT("comp:%s other:%s othercomp:%s normalImp:%s result:%d"),
        HitComp ? *HitComp->GetFName().ToString() : TEXT("null"),
        OtherActor ? *OtherActor->GetFName().ToString() : TEXT("null"),
        OtherComp ? *OtherComp->GetFName().ToString() : TEXT("null"), *NormalImpulse.ToString(), Hit.bBlockingHit);
    */
    LastGroundContact = GetWorld() ? GetWorld()->TimeSeconds : 0.0f;
}

bool UPhysCharacterMovementComponent::IsFalling()
{
    if (!UpdatedPrimitive)
    {
        return false;
    }

    if (auto* world = GetWorld())
    {
        FVector vel = UpdatedPrimitive->GetComponentVelocity();
        if (vel.IsNearlyZero())
        {
            return false;
        }
        return LastGroundContact + 0.1f < world->TimeSeconds;
    }
    else
    {
        return false;
    }
}

void UPhysCharacterMovementComponent::UpdatetOrientation(const FVector& v)
{
    FRotator orientation;
    if (v.X > 1.f)
    {
        orientation.Yaw = 0.0f;
        MoveUpdatedComponent(FVector::ZeroVector, orientation, true);
    }
    else if (v.X < -1.f)
    {
        orientation.Yaw = 180.0f;
        MoveUpdatedComponent(FVector::ZeroVector, orientation, true);
    }
}

