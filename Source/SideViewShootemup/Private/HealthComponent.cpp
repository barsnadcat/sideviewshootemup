#include "HealthComponent.h"

#include "SideViewShootemup/SideViewShootemup.h"
#include "Ship/ShipPart.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    if (AActor* owner = GetOwner())
    {
        owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
    }
}

void UHealthComponent::HandleTakeAnyDamage(AActor* damagedActor, float damage, const class UDamageType* damageType, class AController* instigatedBy, AActor* damageCauser)
{
    /*UE_LOG(Game, Display, TEXT("HandleTakeAnyDamage: damagedActor:%s damage:%.1f damageType:%s instigatedBy:%s damageCauser:%s"),
        damagedActor ? *damagedActor->GetFName().ToString() : TEXT("null"), damage,
        damageType ? *damageType->GetFName().ToString() : TEXT("null"),
        instigatedBy ? *instigatedBy->GetFName().ToString() : TEXT("null"),
        damageCauser ? *damageCauser->GetFName().ToString() : TEXT("null"));*/
    Health -= damage;
    if (Health < 0)
    {
        UE_LOG(Game, Display, TEXT("%s died"),
            damagedActor ? *damagedActor->GetFName().ToString() : TEXT("null"));
        /*if (AShipPart* part = GetOwner<AShipPart>())
        {
            part->BreakAndReweldShip();
        }*/
        GetOwner()->Destroy();
    }
}