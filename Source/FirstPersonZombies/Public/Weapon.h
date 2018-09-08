// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimSequence.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.h"
#include "FPSCharacter.h"
#include "Weapon.generated.h"

class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class AFPSCharacter;

UCLASS()
class FIRSTPERSONZOMBIES_API AWeapon : public AActor
{
	GENERATED_BODY()

private:
	FTimerHandle FireEndTimerHandle;

	void FireEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// First PErson mesh (arms), visible only to owning player
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMesh;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<class AFPSProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	UTexture2D* Display;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (EditCondition = "HasScope"))
	UTexture2D* ScopeTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	TSubclassOf<class UCameraShake> CameraShake;

public:
	// Sets default values for this actor's properties
	AWeapon();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//////////////////////////////////////////////////////// Animations
	/** AnimMontage to play each time we fire */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay")
		UAnimSequence* FireAnimation;

	/** AnimMontage to play each time we reload */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay")
		UAnimSequence* ReloadAnimation;

	/** AnimMontage to play each time we pump, cock, or otherwise ready a new bullet for firing */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay", meta = (EditCondition = "IsIndividuallyLoaded"))
		UAnimSequence* PumpAnimation;

	/** AnimMontage to play each time we pump, cock, or otherwise ready a new bullet for firing */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay", meta = (EditCondition = "IsIndividuallyLoaded"))
		UAnimSequence* IndividualReloadStartAnimation;

	/** AnimMontage to play each time we pump, cock, or otherwise ready a new bullet for firing */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay", meta = (EditCondition = "IsIndividuallyLoaded"))
		UAnimSequence* IndividualReloadEndAnimation;

	///////////////////////////////////////////////////////////////////////// Functions

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void Fire();
	
	UFUNCTION()
		void ReloadIndividual(float Time);

	UFUNCTION()
		void Reload();
	UFUNCTION()
		void ReloadMagazine();

	UFUNCTION()
		void Pump();

	UFUNCTION()
		void TriggerPressed();

	UFUNCTION()
		void TriggerReleased();

	UPROPERTY(BlueprintReadOnly)
		AFPSCharacter* MyFPSCharacter = NULL;

	//////////////////////////////////////////////////////////////////////////// Stats

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")		
	float MinimumFireTime = .1;
	
	// Whether get a scope overlay while aiming down the sights.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		bool HasScope = false;

	// Is this weapon automatic or semi-automatic
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		bool Automatic = false;

	// Whether this weapon fits in the sidearm slot
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		bool IsSideArm = false;

	// Whether bullets are loaded one at a time
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		bool IsIndividuallyLoaded = false;

	// Whether we need to play a pump-action/bolt-action animation after firing
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		bool IsPumpAction = false;

	// How many bullets fit in the magazine/clip/tube
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		int ProjectilesPerShot = 1;

	// How many bullets fit in the magazine/clip/tube
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		int MagazineMax = 7;

	// How much ammo we get when we start or get a max ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		int AmmoMax = 49;

	// How long it takes before we can shoot again
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float ShootInterval = 0.1f;

	// How long it takes us to reload
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float ReloadTime = 2.0;

	// How long it takes us to Pump
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (EditCondition = "IsPumpAction"))
		float PumpTime = 2.0;

	// How much our camera will go up when we shoot
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float RecoilVertical = 1;

	// How fast we equip
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float EquipSpeed = .05;

	// How fast we dequip
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float DequipSpeed = .05;

	// How fast we dequip
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float EquipLowerAmount = 50;

	// How much our camera can sway when we shoot
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float RecoilHorizontal = .5;

	// How much our bloom increases when we shoot
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float Bloom = 0.35;

	// How fast our bloom decreases
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float BloomDecay = 0.9;

	// Our minimum bloom
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float BloomBase = 0.0;

	// How wide our reticle is at 0 bloom
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		float BloomSizeMin = 10;

	// How fast our reticle grows in size
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
		float BloomSizeScale = 50;

	// 0 to 1 how much aiming down the sights lowers bloom
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float AimDownSightSteadyScale = 0.5;

	// How fast we aim
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float AimSpeed = 3;

	// How far we zoom in when aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float ZoomAmount = 20;

	// Scales how much our bloom effects our aim
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float BloomAmount = .03;

	// How much damage a bullet does upon hitting an object
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float Damage = 12.5;

	// How many different zombies we can hit before our bullet is destroyed
	UPROPERTY(EditAnywhere, Category = "Weapon")
		int Pierce = 3;

	// Enable for bullet traces
	UPROPERTY(EditAnywhere, Category = "Weapon")
		bool Debug = false;

	// The offset that our gun uses while at the hip
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		FVector GunOffset = FVector(75.0f, 45.0f, 0.0f);

	// The offset that our gun uses while aiming down the sights
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		FVector GunOffset_Sights = FVector(75.0f, 45.0f, 0.0f);

	UPROPERTY(BlueprintReadWrite)
		int AmmoCurrent = AmmoMax;
	
	UPROPERTY(BlueprintReadWrite)
	int MagazineCurrent = MagazineMax;
	
	UPROPERTY(BlueprintReadWrite)
	bool Reloading = false;
	
	UPROPERTY(BlueprintReadWrite)
		bool Pumping = false;

	UPROPERTY(BlueprintReadWrite)
		bool Pumped = true;

	UPROPERTY(BlueprintReadWrite)
		bool Firing = false;

	UPROPERTY(BlueprintReadWrite)
		bool TriggerPulled = false;

	UPROPERTY(BlueprintReadWrite)
		bool JustFired = false;

	UPROPERTY(BlueprintReadWrite)
	int IndividualReloadIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	float IndividualReloadTimer = 0;

	UPROPERTY(BlueprintReadWrite)
	float BloomCurrent = 0.0;

	float PumpTime_Current = PumpTime;

	float ShootIntervalCurrent = ShootInterval;

	float ReloadTime_Current = ReloadTime;

	float BloomCurrent_Previous = BloomCurrent;
	


	
	
};
