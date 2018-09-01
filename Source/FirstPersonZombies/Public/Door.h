// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONZOMBIES_API ADoor : public AInteractableActor
{
	GENERATED_BODY()
	public:
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "_Window")
		bool Open = false;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "_Window")
		int Price = 5;
	
};
