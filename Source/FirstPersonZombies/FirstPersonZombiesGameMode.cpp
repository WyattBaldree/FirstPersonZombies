// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FirstPersonZombiesGameMode.h"
#include "FirstPersonZombiesHUD.h"
#include "FirstPersonZombiesCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFirstPersonZombiesGameMode::AFirstPersonZombiesGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFirstPersonZombiesHUD::StaticClass();
}
