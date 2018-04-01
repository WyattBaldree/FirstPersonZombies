// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponShop.generated.h"

UCLASS()
class FIRSTPERSONZOMBIES_API AWeaponShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// First PErson mesh (arms), visible only to owning player
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Projectile class to spawn.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay")
	int Price = 0;
	
};
