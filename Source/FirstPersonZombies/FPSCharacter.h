// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Weapon.h"
#include "ZombieSpawner.h"
#include "Components/CapsuleComponent.h"
#include "InteractableActor.h"
#include "FPSCharacter.generated.h"


class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;
class AWeapon;
class AFPSCharacter;

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

	/** Weapon we start with */
	UPROPERTY(EditAnywhere, Category = "Gameplay")
	TSubclassOf<class AWeapon> StartingWeapon;

	// capsule collision component
	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay")
		class UCapsuleComponent* InteractCapsuleComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* FPSCameraComponent;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	////////////////////////////////////////////////////////////////////////////////////////////// My Hud
	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	TSubclassOf<class UUserWidget> wMyHud;

	// Variable to hold the widget After Creating it.
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	UUserWidget* MyHud;

	////////////////////////////////////////////////////////////////////////////////////////////// Misc

	UFUNCTION()
	void DebugWyatt();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	AZombieSpawner* DebugZombieSpawner;

	////////////////////////////////////////////////////////////////////////////////////////////// Interactable
	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	TArray<AInteractableActor*> InteractableArray;

	UPROPERTY(BlueprintReadOnly)
		bool Interacting = false;

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

	UPROPERTY(VisibleDefaultsOnly, Category = "Gameplay")
	UCapsuleComponent* InteractCapsule;

	AInteractableActor* CurrentInteractableActor = nullptr;


	////////////////////////////////////////////////////////////////////////////////////////////// Cash

	// Your current money
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int Cash = 0;

	// This function behaves as an event. If this event has not been used in a blueprint, it will perform the EventName_Implementation code in the cpp file. 
	// EventName_Implementation cannot be overridden, so in order to have different functionality in child classes, you must instead create and override an overridable function which is called by Interact_Implementation.
	// If this event has been used in a blueprint, the actor will completely ignore the Interact_Implementation code and use the blueprint functionality instead.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void CashUpdateEvent(const int NewValue); // The name of our Event in unreal. The const parameter value appears as an input on the blueprint event.
	void CashUpdateEvent_Implementation(int NewValue); // This must be declared in order for the Interact_Implementation to work properly 

	// Switch to your sidearm
	UFUNCTION(BlueprintCallable)
		bool DoTransaction(int Price);

	UFUNCTION(BlueprintCallable)
		void SetCash(int amount);

	UFUNCTION(BlueprintCallable)
		int GetCash();

	////////////////////////////////////////////////////////////////////////////////////////////// Weapons

	// These functions behave as events in the blueprint editor. If this event has not been used in a blueprint, it will perform the EventName_Implementation code in the cpp file. 
	// EventName_Implementation cannot be overridden, so in order to have different functionality in child classes, you must instead create and override an overridable function which is called by Interact_Implementation.
	// If this event has been used in a blueprint, the actor will completely ignore the Interact_Implementation code and use the blueprint functionality instead.
	
	// This event is called any time we need to update what the 2d scope texture looks like
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ScopeTextureUpdateEvent(); // The name of our Event in unreal. The const parameter value appears as an input on the blueprint event.
	void ScopeTextureUpdateEvent_Implementation(); // This must be declared in order for the Interact_Implementation to work properly 

	// This function is called any time we need to update our gun's zoom
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ScopeOpacityUpdateEvent(const float NewOpacity); // The name of our Event in unreal. The const parameter value appears as an input on the blueprint event.
	void ScopeOpacityUpdateEvent_Implementation(float NewOpacity); // This must be declared in order for the Interact_Implementation to work properly

	// This function is called any time we need to update our gun's Bloom
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void BloomUpdateEvent(const float NewBloom, const AWeapon* Weapon); // The name of our Event in unreal. The const parameter value appears as an input on the blueprint event.
	void BloomUpdateEvent_Implementation(float NewBloom, AWeapon* Weapon); // This must be declared in order for the Interact_Implementation to work properly
	
	// This function is called any time we need to update our gun's Reticle Opacity
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void ReticleOpacityUpdateEvent(const float NewOpacity); // The name of our Event in unreal. The const parameter value appears as an input on the blueprint event.
	void ReticleOpacityUpdateEvent_Implementation(float NewOpacity); // This must be declared in order for the Interact_Implementation to work properly

	// Currently held weapon (sidearm or weapon)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		AWeapon* HeldWeapon;

	// SideArm slot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		AWeapon* SideArm;

	// Weapon slots (does not include the sidearm)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TArray<AWeapon*> Weapons;

	// Wepon slots (does not include the sidearm)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float AimDownSightsAmount = 0.0;

	// Wepon slots (does not include the sidearm)
	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
	float AimDownSightsAmount_Current = AimDownSightsAmount;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
		bool AimingDownSights = false;

	int adstoggle = 1;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float PlayerFieldOfView = 90;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
		bool Firing = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
		bool TriggerPulled = false;

	// You currently chosen weapon (but not including sidearms)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		int CurrentWeapon = 0;

	// Are we currently holding the sidearm?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		bool HoldingSideArm = true;

	UFUNCTION()
		void Reload();

	UFUNCTION()
		void CrouchStart();

	UFUNCTION()
		void CrouchStop();

	// Switch to a weapon
	UFUNCTION(BlueprintCallable)
		void SwitchWeapon(int WeaponIndex, bool EquipSideArm = false);

	// Switch to a weapon
	UFUNCTION(BlueprintCallable)
		void SwitchWeaponStart(int WeaponIndex, bool EquipSideArm = false);

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
		void EquipWeapon(AWeapon* weapon, int WeaponIndex, bool EquipSideArm = false);

	/*// Equip a new weapon to one of your SideArm slot
	UFUNCTION(BlueprintCallable)
		void EquipSideArm(AWeapon* weapon);*/

	// equip a new weapon to one of your weapon slots after a delay
	UFUNCTION(BlueprintCallable)
		void EquipStart(AWeapon* weapon, int WeaponIndex, bool EquipSideArm = false);

	// The current equip timer.
	UPROPERTY(BlueprintReadOnly, Category = "Camera")
		float EquipAmount;

	// The current equip timer.
	UPROPERTY(BlueprintReadOnly, Category = "Camera")
		float Dequiping = false;

	AWeapon* EquipWeaponTarget;
	int WeaponIndexTarget;
	bool EquipSideArmTarget;
	bool Switching;


	////////////////////////////////////////////////////////////////////////////////////////////// Controls

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

	UFUNCTION()
		void AimPressed();

	UFUNCTION()
		void AimReleased();

};
