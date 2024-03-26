#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "HealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIDEVIEWSHOOTEMUP_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleTakeAnyDamage(AActor* damagedActor, float damage, const UDamageType* damageType, AController* instigatedBy, AActor* damageCauser);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game);
    int Health = 100;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game);
    int MaxHealth = 100;
};
