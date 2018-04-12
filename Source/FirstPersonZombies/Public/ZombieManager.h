// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSZombie.h"
#include "ZombieSpawner.h"
#include "ZombieManager.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EWaveStateEnum : uint8
{
	VE_Waiting 	UMETA(DisplayName = "Waiting"),
	VE_Starting 	UMETA(DisplayName = "Starting"),
	VE_InProgress 	UMETA(DisplayName = "InProgress"),
	VE_Ending		UMETA(DisplayName = "Ending")
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AFPSZombie*> ZombieList;

	UPROPERTY(EditAnywhere)
	TArray<AZombieSpawner*> ZombieSpawnerList;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<class AFPSZombie> ZombieClass;

	/*************** Wave Functions & Variables ******************/

	/** Call to start the current Wave. */
	UFUNCTION()
	void StartWave();

	/** Called when the CurrentWaveCount reaches 0. */
	UFUNCTION()
	void EndWave();

	/** Represents the current state of the game. */
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	EWaveStateEnum WaveState = EWaveStateEnum::VE_Waiting;

	/** the wave the player is currently on. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	int Wave = 1;

	/** The number of zombies in the current wave, when a this number reaches 0, the wave is over. */
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	int CurrentWaveCount = 0;

	/** The number of zombies we still need to spawn for the current wave, when a this number reaches 0, we stop zpawning zombies. */
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	int CurrentWaveSupply = 0;

	/** This function returns the spawn interval for the wave that is passed. */
	UFUNCTION()
	float GetSpawnInterval(int CurrentWave);

	/** The amount of time in between zombies spawning for the first wave. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	float SpawnIntervalBase = 4.0f;

	/** The change in time in between zombies spawning for each consecutive wave. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	float SpawnIntervalScaling = -0.25f;

	/** Variable used for timing zombie spawns. */
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	float SpawnTimer = 0;

	/** this is the maximum number of zombies that can be in the map at a time. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float MaxConcurrentZombies = 27;

	/** The max number of spawners we will randomly choose from when choosing where to spawn a zombie.
		The higher the number, the farther away zombies can spawn. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	int NumRandomSpawners = 10;

	/****************** Wave Size *********************/

	/** This function returns the number of zombies that should spawn before a wave is over. */ 
	UFUNCTION()
	float GetWaveSize(int CurrentWave);

	/** This is the number of zombies that will spawn during the first wave. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float WaveSizeBase = 8;

	/** This is how many more zombies will spawn after each wave. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float WaveSizeScaling = 4;

	/****************** Zombie Health *********************/

	/** This function returns the health a zombie should have based on the parameter: CurrentWave. */
	UFUNCTION()
	float GetZombieHealth(int CurrentWave);

	/** This is the health of the zombies during the first wave. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float ZombieHealthBase = 50;

	/** This is the amount of health the zombies gain after each wave. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float ZombieHealthScaling = 50;

	/****************** Spawn Functions *********************/

	/** This function spawns a zombie at the spawner passed as a parameter. */
	UFUNCTION()
	bool SpawnZombie(AZombieSpawner* Spawner);

	/** This function creates a new zombie at a semi-random spawner. */
	UFUNCTION()
	bool NewZombie();

	/** This selects a semi-random ZombieSpawner based on the player's location and whether the spawner is active or not. */
	UFUNCTION()
	AZombieSpawner* PickSpawner();

	/****************** Death Functions *********************/
	
	/** This function tells each currently spawned zombie to kill itself */
	UFUNCTION(BlueprintCallable)
	void KillAll();
	

	// Struct used for sorting the zombie spawners by distance from player.
	struct SpawnerSort
	{
		bool operator()(const AZombieSpawner* A, const AZombieSpawner* B) const
		{
			return A->DistanceFromPlayer > B->DistanceFromPlayer;
		}
	};

	

private:
	//This function sorts the array holding the zombie spawners by distance from player.
	void UpdateSpawnerDistances();

	
};
