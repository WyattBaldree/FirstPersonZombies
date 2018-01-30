// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSProjectGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONZOMBIES_API AFPSProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

		virtual void StartPlay() override;
	
	
};
