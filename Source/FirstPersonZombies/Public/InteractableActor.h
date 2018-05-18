// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

//The purpose of this call is to be extended by other blueprints or classes.

UCLASS()
class FIRSTPERSONZOMBIES_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// This function allows us to give child classes different functionality when the Interact_Implementation is called since Interact_Implementation cannot be virtual.
	virtual void InteractInternal();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// This function behaves as an event. If this event has not been used in a blueprint, it will perform the Interact_Implementation code in InteractableActor.cpp. 
	// Interact_Implementation cannot bee overridden so in order to have different functionality, you must instead override InteractionInternal which is called by Interact_Implementation.
	// If this event has been used in a blueprint, the actor will completely ignore the Interact_Implementation code and use the blueprint functionality instead.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "A_Interactable")
		void Interact();
		void Interact_Implementation(); // This must be declared in order for the Interact_Implementation to work properly 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Interactable")
		FString InteractableString = "Default interactable string...";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Interactable")
		float InteractableDistance = 50;

};
