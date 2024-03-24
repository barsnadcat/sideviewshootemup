#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "MyPlayerController.generated.h"

class APhysCharacterPawn;
/**
 *
 */
UCLASS()
class SIDEVIEWSHOOTEMUP_API AMyPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputMappingContext> ShipInputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputMappingContext> CharacterInputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> ThrustVectorAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> ShootAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> InteractAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> CancelInteractionAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    TObjectPtr<UInputAction> MoveAction;

public:
    virtual void SetupInputComponent() override;
    virtual void PlayerTick(float DeltaTime) override;
    virtual void SetPawn(APawn* InPawn) override;

    void SetDefaultPawn(APawn* defaultPawn);

private:
    void OnTrustVectorTriggered(const FInputActionInstance& thrustVector);
    void OnMoveTriggered(const FInputActionInstance& thrustVectorAction);
    void OnShootTriggered();
    void OnInteractTriggered();
    void OnCancelInteractionTriggered();

    FVector GetMouseWorldPosition(double planeY);

    TObjectPtr<APhysCharacterPawn> DefaultPawn;
};
