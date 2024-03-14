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
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

};
