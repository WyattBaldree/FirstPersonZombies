// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LineTraceActor.generated.h"

UCLASS()
class FIRSTPERSONZOMBIES_API ALineTraceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALineTraceActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void NotifyEvent();

	/*
	This Function creates a Raycast and fills the given HitResult
	@param Hit: This Variable will be filled with the Information one needs.
	@return bool value which specifies IF something was hit
	*/
	bool LineTraceFromCamera(FHitResult& Hit);
	
private:
	//A Reference to the Player Camera
	APlayerController* PC;
};
