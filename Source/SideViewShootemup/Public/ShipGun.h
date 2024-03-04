#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ShipGun.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SIDEVIEWSHOOTEMUP_API UShipGun : public USceneComponent
{
    GENERATED_BODY()

public:
    UShipGun();

    void OnAimAt(const FVector& target, float deltaTime);
    void OnShoot();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    double GunTraking = 180.0f;

    UPROPERTY(EditDefaultsOnly, Category = Game)
    TSubclassOf<class AActor> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    TObjectPtr<USceneComponent> GunMuzzle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
    double RPM = 180.f;

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
    double mLastShootSeconds = 0.0f;
};
