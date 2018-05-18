// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableActor.h"


// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

//This is what happens on the Event if we have not created an event in the child blueprint or class 
void AInteractableActor::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Default event function."))
	InteractInternal();
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::InteractInternal()
{
	UE_LOG(LogTemp, Warning, TEXT("Default internal interact function."))
}
