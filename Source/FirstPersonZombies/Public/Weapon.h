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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	UTexture2D* Display;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	bool Automatic = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	bool IsSideArm = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int AmmoMax = 49;
	UPROPERTY(BlueprintReadWrite)
	int AmmoCurrent = AmmoMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MagazineMax = 7;
	UPROPERTY(BlueprintReadWrite)
	int MagazineCurrent = MagazineMax;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float ShootInterval = 1.0;
	float ShootIntervalCurrent = ShootInterval;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float ReloadTime = 2.0;
	float ReloadTimeCurrent = 0.0;
	bool Reloading;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float Bloom = 0.25;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float BloomDelta = 0.01;
	UPROPERTY(BlueprintReadWrite)
	float BloomCurrent = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector GunOffset;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire(bool TriggerPulled);

	UFUNCTION()
	void Reload();
};
