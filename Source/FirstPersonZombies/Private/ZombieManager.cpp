// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieManager.h"


// Sets default values
AZombieManager::AZombieManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZombieManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombieManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*// Iterate htrough all NPCs
	for (int i = 0; i < ZombieList.Num(); i++) {
		AFPSZombie* CurrentZombie = ZombieList[i];
		// If the current NPC is not busy doing something else
		/*if (CurrentZombie->Busy == false) {
			// Create an Array to hold all possible workstations the NPC can enter.
			TArray<AZombieSpawner*> WorkstationArray;
			// Fill WorkstationArray with all possible workstations
			for (AZombieSpawner*& Workstation : ZombieSpawnerList) {
				// If our workstation accepts NPCs with our current NPCs type
				if (Workstation->AcceptedNPCTypes.Contains(CurrentZombie->NPCType) && Workstation->Occupied == false) {
					WorkstationArray.Add(Workstation);
				}

				if (WorkstationArray.Num() > 0) {

				}
			}
		}

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, GetDebugName(ZombieList[i]));
		}

	}*/

	// Iterate through all zombie spawners
	/*for (int i = 0; i < ZombieSpawnerList.Num(); i++) {
		AZombieSpawner* CurrentSpawner = ZombieSpawnerList[i];

		FVector SpawnerLocation = CurrentSpawner->GetActorLocation();
		FRotator SpawnRotation = CurrentSpawner->GetActorRotation();

		MyZombieManager = World->SpawnActor<AZombieManager>(ZombieManagerClass, FVector(0.f, 0.f, 0.f), FRotator::ZeroRotator, Parameters);
	}*/
}

void AZombieManager::SpawnZombie(AZombieSpawner* Spawner)
{
	FVector SpawnerLocation = Spawner->GetActorLocation();
	FRotator SpawnerRotation = Spawner->GetActorRotation();

	if (ZombieClass) {
		UWorld* const World = GetWorld();

		if (World != NULL)
		{
			FActorSpawnParameters Parameters;
			Parameters.Owner = this;
			Parameters.Instigator = Instigator;

			AFPSZombie* NewZombie = World->SpawnActor<AFPSZombie>(ZombieClass, SpawnerLocation, SpawnerRotation, Parameters);

			if (NewZombie) {
				// We have to shift the zombie up to prevent them from clipping through the floor.
				FVector ZombieBoundingBoxCenter;
				FVector ZombieBoundingBoxExtent;
				NewZombie->GetActorBounds(true, ZombieBoundingBoxCenter, ZombieBoundingBoxExtent);

				FVector ZombieRootLocation = NewZombie->GetRootComponent()->GetComponentLocation();

				float SpawnShift = ZombieBoundingBoxExtent.Z - (ZombieBoundingBoxCenter.X - ZombieRootLocation.Z);

				NewZombie->AddActorLocalOffset(FVector(0.0, 0.0, SpawnShift));
				
			}
			else
			{
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Problem spawning a new zombie in the method SpawnZombie."));
				}
			}
		}
	}
	else
	{
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("No ZombieClass set in ZombieManager."));
		}
	}

	//MyZombieManager = World->SpawnActor<AZombieManager>(ZombieManagerClass, FVector(0.f, 0.f, 0.f), FRotator::ZeroRotator, Parameters);
}

