// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "InteractableActorChild.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONZOMBIES_API AInteractableActorChild : public AInteractableActor
{
	GENERATED_BODY()
	
protected:
	// In order to extend the event functionality we only need to override the InteractIntertnal function, not the Interact_Implementation function.
	virtual void InteractInternal(AActor* InteractActor) override;
	
	
};
