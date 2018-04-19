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
	StartWave();
}

// Called every frame
void AZombieManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, TEXT("CurrentWaveCount: ") + FString::FromInt(CurrentWaveCount));
		GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Yellow, TEXT("CurrentWaveSupply: ") + FString::FromInt(CurrentWaveSupply));
		GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Yellow, TEXT("Wave: ") + FString::FromInt(Wave));

		switch (WaveState) {
		case EWaveStateEnum::VE_Waiting:
			GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Yellow, TEXT("WaveState =  VE_Waiting"));
			break;
		case EWaveStateEnum::VE_Starting:
			GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Yellow, TEXT("WaveState =  VE_Starting"));
			break;
		case EWaveStateEnum::VE_InProgress:
			GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Yellow, TEXT("WaveState =  VE_InProgress"));
			break;
		case EWaveStateEnum::VE_Ending:
			GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Yellow, TEXT("WaveState =  VE_Ending"));
			break;
		}

		GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Yellow, TEXT("SpawnTimer: ") + FString::SanitizeFloat(SpawnTimer));
	}



	// If there is a wave in progress
	if (WaveState == EWaveStateEnum::VE_InProgress) {

		if (CurrentWaveSupply > 0) {
			// increment our zombie spawning timer
			SpawnTimer += DeltaTime;
			if (SpawnTimer >= GetSpawnInterval(Wave)) {
				SpawnTimer -= GetSpawnInterval(Wave);

				// Attempt to spawn a zombie 100 times.
				int i = 0;
				while (!NewZombie()) {
					i++;
					if (i > 100) {

						break;
					}
				}
			}
		}

		if (CurrentWaveCount <= 0) EndWave();
	}
}

void AZombieManager::StartWave()
{
	WaveState = EWaveStateEnum::VE_Starting;
	CurrentWaveCount = GetWaveSize(Wave);
	CurrentWaveSupply = CurrentWaveCount;

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Starting Wave: ") + FString::FromInt(Wave));
	}

	// Here we would put anything that needs to happen as a wave is starting up.
	// i.e. hud elements and sound effects.

	WaveState = EWaveStateEnum::VE_InProgress;
}

void AZombieManager::EndWave()
{
	WaveState = EWaveStateEnum::VE_Starting;
	// Here we would put anything that needs to happen as a wave is ending.
	// i.e. hud elements and sound effects.
	Wave++;
	// At the end of this wave, start the next.
	StartWave();
}

float AZombieManager::GetSpawnInterval(int CurrentWave)
{
	return SpawnIntervalBase + ((CurrentWave - 1) * SpawnIntervalScaling);
}

float AZombieManager::GetZombieHealth(int CurrentWave)
{
	return ZombieHealthBase + ((CurrentWave - 1) * ZombieHealthScaling);
}

float AZombieManager::GetWaveSize(int CurrentWave)
{
	return WaveSizeBase + ((CurrentWave - 1) * WaveSizeScaling);
}

bool AZombieManager::SpawnZombie(AZombieSpawner* Spawner)
{
	if (Spawner == NULL) return false;

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
				NewZombie->TargetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Unable to spawn zombie at zombie spawner: %s"), *GetNameSafe(Spawner));
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

	if (ActiveZombieSpawners.Num() <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Unable to find any active zombie spawners."));
		return NULL;
	}

	// Sort the list of active spawners by the distance variable they hold.
	Algo::Sort(ActiveZombieSpawners, SpawnerSort());

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
	if (SpawnZombie(PickSpawner())) {
		CurrentWaveSupply--;
		return true;
	}
	else {
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, TEXT("Trouble spawning a zombie in function NewZombie()"));
		}
		return false;
	}
	
}

void AZombieManager::KillAll()
{
	for(auto& zombie : ZombieList)
		zombie->ZombieState = ZombieStateEnum::VE_Dead;
}