// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponShop.h"
#include "DummyWeaponShop.generated.h"

UCLASS()
class FIRSTPERSONZOMBIES_API ADummyWeaponShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADummyWeaponShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<TSubclassOf<class AWeaponShop>> WeaponShopClasses;
	
};
