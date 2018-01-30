// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class USkeletalMeshComponent;

UCLASS()
class FIRSTPERSONZOMBIES_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FPSCameraComponent;

	// First PErson mesh (arms), visible only to owning player
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* FPSMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UAnimSequence* FireAnimation;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	USoundBase* FireSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	void Fire();

	bool bFiring;

	UPROPERTY()
	int MaxAmmo = 11;
	int CurrentAmmo = MaxAmmo;


	bool AnimStartFiring = false;

	UFUNCTION(BlueprintCallable)
	bool GetAnimStartFiring();

	UFUNCTION(BlueprintCallable)
	void SetAnimStartFiring(bool b);

	
};
