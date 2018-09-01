// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableActorChild.h"

// Here, we override the Interal function rather than InteractInternal since only this function can be virtual. 
// This function will automatically be called by Interact_Implementation
void AInteractableActorChild::InteractInternal(AActor* InteractActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Overriden internal interact function."))
}
