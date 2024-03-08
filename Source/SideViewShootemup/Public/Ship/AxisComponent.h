#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AxisComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIDEVIEWSHOOTEMUP_API UAxisComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UAxisComponent();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Game)
	double Traking = 180.0f;

	void UpdateDirection(const FVector& vector, float time);
};
