// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSZombie.h"
#include "FirstPersonZombiesGameMode.h"


// Sets default values
AFPSZombie::AFPSZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSZombie::BeginPlay()
{
	Super::BeginPlay();
	
	//When this Zombie is created, add it to the ZombieList in our ZombieManager.
	AFirstPersonZombiesGameMode* GameMode = (AFirstPersonZombiesGameMode*)GetWorld()->GetAuthGameMode();
	AZombieManager* ZombieManager = GameMode->MyZombieManager;

	//if we don't already have an NPC manager, go ahead and create one.
	if (ZombieManager == NULL) {
		GameMode->MakeZombieManager();
		ZombieManager = GameMode->MyZombieManager;
	}
	//Add the zombie to the ZombieList.
	ZombieManager->ZombieList.Add(this);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(ZombieManager->ZombieList.Num()));
	}
}

// Called every frame
void AFPSZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GEngine) {

		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::SanitizeFloat(HP));
	}
}

// Called to bind functionality to input
void AFPSZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AFPSZombie::Hurt(float Damage, bool Headshot)
{
	if (Headshot) {
		HP -= Damage*2.5;
	}else{
		HP -= Damage;
	}

	if (HP <= 0) 
	{
		Destroy();
		return true;
	}

	return false;
}

