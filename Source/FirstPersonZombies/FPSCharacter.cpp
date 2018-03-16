// Fill out your copyright notice in the Description page of Project Settings.
/*
#include "FPSCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"*/

#include "FirstPersonZombies.h"
#include "FPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "FPSProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/PlayerController.h"
#include "FirstPersonZombiesGameMode.h"
#include "ZombieManager.h"
#include "Engine/EngineTypes.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person camera component
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());
	// Set the offset to be approximately in the characters eyes
	FPSCameraComponent->SetRelativeLocation(FVector(0, 0, 50.0f + BaseEyeHeight));
	// Allow the controller to rotate our camera
	FPSCameraComponent->bUsePawnControlRotation = true;
	
	// The owning player doesn't see the regular (third-person) body mesh.
	GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine)
	{
		// Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
	}

	if (StartingWeapon) {
		UWorld* World = GetWorld();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AWeapon* weapon = World->SpawnActor<AWeapon>(StartingWeapon, FVector(0, 0, 0), FRotator(1, 1, 1), SpawnParams);
		EquipWeapon(weapon);
	}
}

void AFPSCharacter::EquipWeapon(AWeapon* weapon)
{
	if (weapon) {
		HeldWeapon = weapon;

		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		HeldWeapon->SetActorLocation(CameraLocation + FTransform(CameraRotation).TransformVector(HeldWeapon->GunOffset));


		HeldWeapon->AttachToComponent(FPSCameraComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up "movement" bindings
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPSCharacter::Reload);

	PlayerInputComponent->BindAction("DebugWyatt", IE_Pressed, this, &AFPSCharacter::DebugWyatt);
}

void AFPSCharacter::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}
void AFPSCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::StartJump() {
	bPressedJump = true;
}

void AFPSCharacter::StopJump() {
	bPressedJump = false;
}

void AFPSCharacter::Fire()
{
	if (HeldWeapon) {
		HeldWeapon->Fire();
	}
}

void AFPSCharacter::Reload()
{
	if (HeldWeapon) {
		HeldWeapon->Reload();
	}
}

void AFPSCharacter::DebugWyatt()
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DebugWyatt!"));
	}

	if (DebugZombieSpawner == NULL) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DebugZombieSpawner not set!"));
		}
		return;
	}


	AFirstPersonZombiesGameMode* GameMode = (AFirstPersonZombiesGameMode*)GetWorld()->GetAuthGameMode();
	AZombieManager* ZombieManager = GameMode->MyZombieManager;

	if (ZombieManager) {
		ZombieManager->SpawnZombie(DebugZombieSpawner);
	}
	else
	{
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ZombieManager does not exist!"));
		}
	}
}
