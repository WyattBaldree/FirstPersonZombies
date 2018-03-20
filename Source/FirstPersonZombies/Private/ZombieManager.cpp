// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"


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
}

float AZombieManager::GetZombieHealth(int CurrentWave)
{
	return ZombieHealthBase + ((ZombieHealthScaling - 1) * CurrentWave);
}

float AZombieManager::GetWaveSize(int CurrentWave)
{
	return WaveSizeBase + ((WaveSizeScaling - 1) * CurrentWave);
}

bool AZombieManager::SpawnZombie(AZombieSpawner* Spawner)
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
				//float CapsuleHalfHeight = NewZombie->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				//NewZombie->AddActorLocalOffset(FVector(0.0, 0.0, CapsuleHalfHeight));

				NewZombie->TargetActor = Spawner->InitialTargetActor;
				NewZombie->MaxHP = GetZombieHealth(Wave);
				NewZombie->HP = NewZombie->MaxHP;
				return true;
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

	return false;
}

void AZombieManager::UpdateSpawnerDistances()
{
	for (int i = 0; i < ZombieSpawnerList.Num(); i++) {
		ZombieSpawnerList[i]->UpdateDistance();
	}
}

AZombieSpawner* AZombieManager::PickSpawner()
{
	// Update the DistanceFromLPayer variable in the spawners for sorting later on.
	UpdateSpawnerDistances();

	// Fill a new array with all of the currently active zombie spawners.
	TArray<AZombieSpawner*> ActiveZombieSpawners;
	for (int i = 0; i < ZombieSpawnerList.Num(); i++) {
		if (ZombieSpawnerList[i]->IsActive == true) {
			ActiveZombieSpawners.Add(ZombieSpawnerList[i]);
		}
	}

	for (int i = 0; i < ZombieSpawnerList.Num(); i++) {
		//UE_LOG(LogTemp, Warning, TEXT("%s ::: %f"), *GetNameSafe(ActiveZombieSpawners[i]), ActiveZombieSpawners[i]->DistanceFromPlayer);
	}

	// Sort the list of active spawners by the distance variable they hold.
	Algo::Sort(ActiveZombieSpawners, SpawnerSort());

	for (int i = 0; i < ActiveZombieSpawners.Num(); i++) {
		//UE_LOG(LogTemp, Warning, TEXT("%s ::: %f"),*GetNameSafe(ActiveZombieSpawners[i]), ActiveZombieSpawners[i]->DistanceFromPlayer);
	}

	int ChosenSpawnerIndex;
	if (ActiveZombieSpawners.Num() < NumRandomSpawners) {
		ChosenSpawnerIndex = FMath::RandRange(0, ActiveZombieSpawners.Num() - 1);
	}
	else{
		ChosenSpawnerIndex = FMath::RandRange(0, NumRandomSpawners - 1);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetNameSafe(ActiveZombieSpawners[ChosenSpawnerIndex]));

	return ActiveZombieSpawners[ChosenSpawnerIndex];
}

bool AZombieManager::NewZombie()
{
	return SpawnZombie(PickSpawner());
}

