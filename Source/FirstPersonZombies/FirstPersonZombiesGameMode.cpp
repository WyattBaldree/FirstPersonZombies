// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FirstPersonZombiesGameMode.h"
#include "FirstPersonZombiesHUD.h"
#include "UObject/ConstructorHelpers.h"

AFirstPersonZombiesGameMode::AFirstPersonZombiesGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

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
}

void AFirstPersonZombiesGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Blue, ZombieManagerClass == NULL ? TEXT("true") : TEXT("false"));
	}
}
