// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSZombie.h"
#include "ZombieSpawner.h"
#include "ZombieManager.generated.h"

UCLASS()
class FIRSTPERSONZOMBIES_API AZombieManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<AFPSZombie*> ZombieList;

	UPROPERTY(EditAnywhere)
	TArray<AZombieSpawner*> ZombieSpawnerList;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<class AFPSZombie> ZombieClass;
	
	UFUNCTION()
	void SpawnZombie(AZombieSpawner* Spawner);

};
