#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIDEVIEWSHOOTEMUP_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game);
    int Health = 100;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game);
    int MaxHealth = 100;
};
