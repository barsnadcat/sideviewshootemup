// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PhysCharacterMovementComponent.h"



void UPhysCharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	FVector InputVector = ConsumeInputVector();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!UpdatedPrimitive)
	{
		return;
	}

	UpdatedPrimitive->AddForce(InputVector, NAME_None, true);

}