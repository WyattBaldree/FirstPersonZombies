// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon.h"
#include "ZombieSpawner.h"
#include "Components/CapsuleComponent.h"
#include "InteractableActor.h"
#include "FPSCharacter.generated.h"


class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;

UCLASS()
class FIRSTPERSONZOMBIES_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	

public:
	// Sets default values for this character's properties
	AFPSCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AInteractableActor* GetClosestInteractable();

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FPSCameraComponent;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	TSubclassOf<class AWeapon> StartingWeapon;

	// capsule collision component
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay")
		class UCapsuleComponent* InteractCapsuleComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	////////////////////////////////////////////My Hud
	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	TSubclassOf<class UUserWidget> wMyHud;

	// Variable to hold the widget After Creating it.
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	UUserWidget* MyHud;

	//Handles input for moving forward and backward.
	UFUNCTION()
	void MoveForward(float Value);

	//Handles input for moving right and left
	UFUNCTION()
	void MoveRight(float Value);
	
	//Sets jump flag when key is pressed.
	UFUNCTION()
	void StartJump();

	//Clears jump flag when key is released.
	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void FirePressed();

	UFUNCTION()
	void FireReleased();

	UFUNCTION()
	void InteractPressed();

	UFUNCTION()
	void InteractReleased();

	UPROPERTY(BlueprintReadOnly)
	bool Interacting = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	bool Firing = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	bool TriggerPulled = false;

	UFUNCTION()
	void Reload();

	UFUNCTION()
	void DebugWyatt();

	// Currently held weapon (sidearm or weapon)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	AWeapon* HeldWeapon;

	// SideArm slot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	AWeapon* SideArm;

	// Wepon slots (does not include the sidearm)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	TArray<AWeapon*> Weapons;

	// Switch to a weapon
	UFUNCTION(BlueprintCallable)
	void SwitchWeapon(int WeaponIndex);

	// Switch to your sidearm
	UFUNCTION(BlueprintCallable)
	void SwitchSideArm();

	// Switch to your sidearm
	UFUNCTION(BlueprintCallable)
	void SwitchWeapon1();

	// Switch to your sidearm
	UFUNCTION(BlueprintCallable)
	void SwitchWeapon2();
	
	// Equip a new weapon to one of your 2 wepon slots
	UFUNCTION(BlueprintCallable)
	void EquipWeapon(AWeapon* weapon, int WeaponIndex);

	// Equip a new weapon to one of your SideArm slot
	UFUNCTION(BlueprintCallable)
	void EquipSideArm(AWeapon* weapon);

	// You currently chosen weapon (but not including sidearms)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int CurrentWeapon = 0;

	// Are we currently holding the sidearm?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool HoldingSideArm = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	AZombieSpawner* DebugZombieSpawner;

	////////////////////////////// Interactable
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	TArray<AInteractableActor*> InteractableArray;

	// sweg
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// sweg
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void InteractBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void InteractChanged();

	UFUNCTION(BlueprintCallable)
	AInteractableActor* GetInteracting();

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
		UCapsuleComponent* InteractCapsule;


	AInteractableActor* CurrentInteractableActor = nullptr;
};
