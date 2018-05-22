// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableActor.h"
#include "Engine/CollisionProfile.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractBox"));
	BoxComponent->SetBoxExtent(FVector(20, 20, 20));
	BoxComponent->SetCollisionProfileName(TEXT("FPSPlayerTrigger"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/////////////////////////InteractableString

void AInteractableActor::SetInteractString(FString NewString)
{
	InteractableString = NewString;
}

FString AInteractableActor::GetInteractString()
{
	return InteractableString;
}

/////////////////////// Interact

//This is what happens on the Event if we have not created an event in the child blueprint or class 
void AInteractableActor::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Default event function."))
		InteractInternal();
}

void AInteractableActor::InteractInternal()
{
	UE_LOG(LogTemp, Warning, TEXT("Default internal interact function."))
}


///////////////////////////// InteractOverlap

void AInteractableActor::InteractOverlapInternal()
{
	UE_LOG(LogTemp, Warning, TEXT("Default overlap interact function."))
}

void AInteractableActor::InteractOverlap_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Default overlap event funtion."))
	InteractOverlapInternal();
}

//////////////////////////// InteractBegin

void AInteractableActor::InteractBeginInternal()
{
	UE_LOG(LogTemp, Warning, TEXT("Default begin interact function."));
}

void AInteractableActor::InteractBegin_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Default begin event funtion."));
	InteractBeginInternal();
}

//////////////////////////// InteractEnd

void AInteractableActor::InteractEndInternal()
{
	UE_LOG(LogTemp, Warning, TEXT("Default end interact function."));
}

void AInteractableActor::InteractEnd_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Default end event funtion."));
	InteractEndInternal();
}



void AInteractableActor::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	this->InteractOverlap();
}