// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "Components/SceneComponent.h"
#include "Window.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPERSONZOMBIES_API AWindow : public AInteractableActor
{
	GENERATED_BODY()

public:
	AWindow();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "_Window")
	int NumBoards = 5;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "_Window")
	class USceneComponent* EntryPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "_Window")
	class USceneComponent* ExitPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "_Window")
	class USceneComponent* AttackPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "_Window")
	class USceneComponent* Root;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "_Window")
	bool Busy = false;
};
