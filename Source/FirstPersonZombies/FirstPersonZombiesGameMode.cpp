// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FirstPersonZombiesGameMode.h"
#include "UObject/ConstructorHelpers.h"

AFirstPersonZombiesGameMode::AFirstPersonZombiesGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Assets/Blueprints/FPSCharacter_bp"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	//HUDClass = AFirstPersonZombiesHUD::StaticClass();
}

void AFirstPersonZombiesGameMode::MakeZombieManager() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("BLLLLLAAAHH"));
	}
	UWorld* World = GetWorld();
	if (World)
	{

		if (ZombieManagerClass == NULL){
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("You need to set the ZombieManagerClass in FirstPersonZombiesGameMode!"));
			}
		}
		else 
		{
			FActorSpawnParameters Parameters;
			Parameters.bAllowDuringConstructionScript = true;
			Parameters.bNoFail = true;
			Parameters.Owner = this;
			Parameters.Instigator = Instigator;

			MyZombieManager = World->SpawnActor<AZombieManager>(ZombieManagerClass, FVector(0.f, 0.f, 0.f), FRotator::ZeroRotator, Parameters);
		}

		if (MyZombieManager == NULL) {
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Unable to create an instance of ZombieManager in FirstPersonZombiesGameMode->MakeZombieManager()"));
			}
		}
	}
	MakeLootManager();
}

void AFirstPersonZombiesGameMode::MakeLootManager() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("LOOOOOT!"));
	}
	UWorld* World = GetWorld();
	if (World)
	{

		if (LootManagerClass == NULL) {
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("You need to set the LootManagerClass in FirstPersonZombiesGameMode!"));
			}
		}
		else
		{
			FActorSpawnParameters Parameters;
			Parameters.bAllowDuringConstructionScript = true;
			Parameters.bNoFail = true;
			Parameters.Owner = this;
			Parameters.Instigator = Instigator;

			MyLootManager = World->SpawnActor<ALootManager>(LootManagerClass, FVector(0.f, 0.f, 0.f), FRotator::ZeroRotator, Parameters);
		}

		if (MyLootManager == NULL) {
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Unable to create an instance of LootManager in FirstPersonZombiesGameMode->MakeLootManager()"));
			}
		}
	}
}

void AFirstPersonZombiesGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
