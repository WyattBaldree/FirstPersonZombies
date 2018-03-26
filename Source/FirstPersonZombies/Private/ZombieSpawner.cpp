// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonZombiesGameMode.h"


// Sets default values
AZombieSpawner::AZombieSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();

	AFirstPersonZombiesGameMode* GameMode = (AFirstPersonZombiesGameMode*)GetWorld()->GetAuthGameMode();
	ZombieManagerReference = GameMode->MyZombieManager;

	//if we don't already have a zombie manager, go ahead and create one.
	if (ZombieManagerReference == NULL) {
		GameMode->MakeZombieManager();
		ZombieManagerReference = GameMode->MyZombieManager;
	}
	//Add the zombie to the ZombieList.
	ZombieManagerReference->ZombieSpawnerList.Add(this);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::FromInt(ZombieManagerReference->ZombieSpawnerList.Num()));
	}
	
}

// Called every frame
void AZombieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZombieSpawner::UpdateDistance()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player == NULL) {
		UE_LOG(LogTemp, Warning, TEXT("Found a Player == NULL while using the UpdateDistance function in ZombieSpawner."));
		return;
	}

	FVector PlayerLocation = Player->GetActorLocation();
	FVector MyLocation = GetActorLocation();

	DistanceFromPlayer = FVector::DistSquared(PlayerLocation,MyLocation);
}
