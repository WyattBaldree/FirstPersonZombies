// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombieManager.h"
#include "LootManager.h"
#include "FirstPersonZombiesGameMode.generated.h"

UCLASS(minimalapi)
class AFirstPersonZombiesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFirstPersonZombiesGameMode();

	void MakeZombieManager();
	void MakeLootManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AZombieManager> ZombieManagerClass;
	UPROPERTY(BlueprintReadWrite, Category = "Managers")
	AZombieManager* MyZombieManager;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ALootManager> LootManagerClass;
	UPROPERTY(BlueprintReadWrite, Category = "Managers")
	ALootManager* MyLootManager;
};



