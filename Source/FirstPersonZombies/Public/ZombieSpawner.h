// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSZombie.h"
#include "Window.h"
#include "ZombieSpawner.generated.h"

class AZombieManager;

UCLASS()
class FIRSTPERSONZOMBIES_API AZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "_Spawner")
	AActor* InitialTargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "_Spawner")
	AWindow* InitialTargetWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "_Spawner")
	ZombieStateEnum InitialState = ZombieStateEnum::VE_Spawning;

	UPROPERTY(BlueprintReadWrite, Category = "_Spawner")
	bool IsActive = true;

	// This value is only updated periodically, when needed.
	float DistanceFromPlayer = 0.0f;

	void UpdateDistance();

private:
	AZombieManager* ZombieManagerReference;
	
};
