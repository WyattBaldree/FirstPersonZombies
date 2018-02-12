// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimSequence.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.h"
#include "Weapon.generated.h"

class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;

UCLASS()
class FIRSTPERSONZOMBIES_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	// First PErson mesh (arms), visible only to owning player
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMesh;

	

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

	/** AnimMontage to play each time we reload */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* ReloadAnimation;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	USoundBase* FireSound;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	USoundBase* ReloadSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int magazine_size = 11;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int magazine_max = 7;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float shoot_interval = 1.0;

	int magazine_current = magazine_size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector GunOffset;
	
	UFUNCTION()
	void Fire();

	UFUNCTION()
	void Reload();

	bool Reloading;
};
