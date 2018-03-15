// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Animation/AnimSequence.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person mesh component for the owning player.
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	// Only the owning player sees this mesh.
	GunMesh->SetOnlyOwnerSee(true);
	// Disable some environmental shadowing to preserve the illusion of having a single mesh.
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Fire()
{
	if (ShootIntervalCurrent > 0) return;

	// Get the animation object for the arms mesh
	UAnimInstance* AnimInstance = GunMesh->GetAnimInstance();

	if (Reloading) return;

	if (MagazineCurrent <= 0 && !Reloading) {
		Reload();
		return;
	}

	AnimInstance->StopSlotAnimation(0, "Arms");

	// try and play a firing animation if specified
	if (FireAnimation)
	{
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// Attempt to fire a projectile.
	if (ProjectileClass)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		FVector location;
		FVector direction;

		FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		FVector2D  ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);

		PlayerController->DeprojectScreenPositionToWorld
		(
			ViewportCenter.X,
			ViewportCenter.Y,
			location,
			direction
		);

		//Trnasform MuzzleOffet from camera space to world space
		FVector MuzzleLocation = location;
		FRotator MuzzleRotation = direction.Rotation();
		// skew to point slightly upward
		//MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			//Spawn the projectile at the muzzle
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
				
			}
		}

	}


	// If we fire, expend a single bullet and reset our fire interval.
	MagazineCurrent--;
	ShootIntervalCurrent = ShootInterval;
}

void AWeapon::Reload()
{
	// try to play the reload sound effect
	if (ReloadSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
	}

	// try and play a reload animation if specified
	if (ReloadAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = GunMesh->GetAnimInstance();

		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(ReloadAnimation, "Arms", 0.0f);
		}
	}

	Reloading = true;
	ReloadTimeCurrent = ReloadTime;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Reloading) {
		ReloadTimeCurrent -= DeltaTime;

		if(ReloadTimeCurrent <= 0) {
			Reloading = false;
			MagazineCurrent = MagazineMax;
		}
	}

	ShootIntervalCurrent -= DeltaTime;

	if (GEngine)
	{
		// Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::SanitizeFloat(MagazineCurrent));
	}

		
}

