#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PhysCharacterPawn.generated.h"

class UCapsuleComponent;
class UPhysCharacterMovementComponent;
class UArrowComponent;
class USkeletalMeshComponent;
class AShipPart;
class APlayerController;

UCLASS()
class SIDEVIEWSHOOTEMUP_API APhysCharacterPawn : public APawn
{
    GENERATED_BODY()

public:
    APhysCharacterPawn(const FObjectInitializer& ObjectInitializer);

    /** The main skeletal mesh associated with this Character (optional sub-object). */
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USkeletalMeshComponent> Mesh;

    /** Movement component used for movement logic in various movement modes (walking, falling, etc), containing relevant settings and functions to control movement. */
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPhysCharacterMovementComponent> PhysCharacterMovement;

    /** The CapsuleComponent being used for movement collision (by CharacterMovement). Always treated as being vertically aligned in simple collision check functions. */
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCapsuleComponent> CapsuleComponent;

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


    UFUNCTION(BlueprintCallable, Category = "Transformation")
    virtual FVector GetAcceleration() const;

    UFUNCTION(BlueprintCallable, Category = "Transformation")
    virtual bool IsFalling() const;

    void Interact(APlayerController* playerController);


#if WITH_EDITORONLY_DATA
    /** Component shown in the editor only to indicate character facing */
    UPROPERTY()
    TObjectPtr<UArrowComponent> ArrowComponent;
#endif

protected:
    UPROPERTY()
    TWeakObjectPtr<AShipPart> OverlappedShipPart;
};
