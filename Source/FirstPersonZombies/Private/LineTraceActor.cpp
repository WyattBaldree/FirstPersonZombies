// Fill out your copyright notice in the Description page of Project Settings.

#include "LineTraceActor.h"
#include "InteractableActor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALineTraceActor::ALineTraceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALineTraceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALineTraceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	NotifyEvent();
}

bool ALineTraceActor::LineTraceFromCamera(FHitResult& Hit)
{
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	if (!pc) {
		UE_LOG(LogTemp, Error, TEXT("1"));
		return false;
	}
	auto PlayerCameraManager = pc->PlayerCameraManager;
	if (!PlayerCameraManager) {
		UE_LOG(LogTemp, Error, TEXT("2"));
		return false;
	}
	//UE_LOG(LogTemp, Error, TEXT("blaahhh"));
	FName TraceTag = TEXT("Draw");
	GetWorld()->DebugDrawTraceTag = TraceTag;

	if (PlayerCameraManager)
	{
		FVector CameraForwardVector = PlayerCameraManager->GetActorForwardVector();

		FVector LineStart = PlayerCameraManager->GetCameraLocation();
		FVector LineEnd = LineStart + CameraForwardVector * 1024;

		FCollisionQueryParams Params;
		Params.bTraceComplex = true;
		Params.bReturnPhysicalMaterial = false;
		Params.bReturnFaceIndex = false;
		Params.TraceTag = TraceTag;

		UE_LOG(LogTemp, Error, TEXT("blaahhh"));

		return GetWorld()->LineTraceSingleByChannel(Hit, LineStart, LineEnd, ECollisionChannel::ECC_Visibility, Params);
	}


	UE_LOG(LogTemp, Error, TEXT("PlayerCameraManager is null"));
	return false;
}

void ALineTraceActor::NotifyEvent()
{
	FHitResult Hit;
	if (LineTraceFromCamera(Hit))
	{
		UE_LOG(LogTemp, Error, TEXT("distance: %f"), Hit.Distance);

		AActor* HitActor = Hit.GetActor();
		if (HitActor) {
			if (HitActor->IsA(AInteractableActor::StaticClass())) {

				AInteractableActor* dfsdj = (AInteractableActor*)HitActor;
				//dfsdj->Interact();
			}
		}
		/*if (Hit.GetActor()->Implements<UInteractable>())
		{
			IInteractable::Execute_Interact(Hit.GetActor());
		}*/
	}
}