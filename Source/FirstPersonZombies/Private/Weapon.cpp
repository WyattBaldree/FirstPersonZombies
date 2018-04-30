// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Animation/AnimSequence.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USceneComponent* RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

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
	AmmoCurrent = AmmoMax;
	MagazineCurrent = MagazineMax;

}

void AWeapon::Fire(bool TriggerPulled)
{
	// If we are semi automatic and the trigger wasn't just pulled, don't shoot.
	if (!Automatic && !TriggerPulled) return;

	// If we are not ready to shoot again, return.
	if (ShootIntervalCurrent > 0) return;

	// Get the animation object for the arms mesh
	UAnimInstance* AnimInstance = GunMesh->GetAnimInstance();

	if (Reloading) return;

	if (!Reloading && MagazineCurrent <= 0 && (AmmoCurrent > 0 || IsSideArm)) {
		Reload();
		return;
	}
	
	if(MagazineCurrent <= 0) return;

	AnimInstance->StopSlotAnimation(0, "Arms");

	if (CameraShake) {
		UGameplayStatics::PlayWorldCameraShake(this, CameraShake, GetActorLocation(), 0.0f, 5000.0f);
	}

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

	if (GEngine) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("firing"));
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

		
		// How much are we currently blooming
		float BloomMagnitude = BloomAmount * BloomCurrent;

		// Get a vector orthogonal to our muzzle direction
		FVector OrthogonalMuzzleVector = FVector(1.0f,1.0f,(-direction.X - direction.Y)/direction.Z).GetSafeNormal(.01);
		// Rotate the orthogonal vecotr around our muzzle direction
		FVector OrthogonalMuzzleVectorRotated = OrthogonalMuzzleVector.RotateAngleAxis(FMath::RandRange(0.0f, 360.0f), direction.GetSafeNormal(.01));
		// Make a new vector in a random direction based off of bloom magnitude
		FVector NewDirection = direction.GetSafeNormal(.01).operator+(OrthogonalMuzzleVectorRotated.GetSafeNormal(.000001) * FMath::RandRange(0.0f, BloomMagnitude));

		//Trnasform MuzzleOffet from camera space to world space
		FVector MuzzleLocation = location;
		FRotator MuzzleRotation = NewDirection.Rotation();

		/*// skew to point slightly for bloom
		float BloomMagnitude = BloomAmount * BloomCurrent
		MuzzleRotation.Pitch += BloomAmount * BloomCurrent * (MagazineCurrent % 2) ? 1 : -1;
		MuzzleRotation.Yaw += BloomAmount * BloomCurrent * (MagazineCurrent % 3) ? 1 : -1;*/
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			//Spawn the projectile at the muzzle
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (GEngine) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("firing2"));
			}
			
			if (Projectile)
			{
				FVector LaunchDirection = NewDirection;// MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
				Projectile->Damage = Damage;
				Projectile->Pierce = Pierce;
				
				if(Debug) DrawDebugLine(World, MuzzleLocation, MuzzleLocation.operator+(NewDirection*10000.0f), FColor(255, 0, 0), false, 5, 0, 1.0f);


			}
		}
	}

	// If we fire, expend a single bullet, reset our fire interval, and increase bloom.
	MagazineCurrent--;
	ShootIntervalCurrent = ShootInterval;
	BloomCurrent += Bloom;
	if(BloomCurrent > 1.0)
		BloomCurrent = 1.0;
}

void AWeapon::Reload()
{
	if(MagazineCurrent == MagazineMax || (!IsSideArm && AmmoCurrent <= 0) || Reloading) return;
	
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

	if(Reloading)
	{
		ReloadTimeCurrent -= DeltaTime;

		if(ReloadTimeCurrent <= 0)
		{
			Reloading = false;
			int delta = MagazineMax - MagazineCurrent;
			if(AmmoCurrent >= delta || IsSideArm)
			{
				MagazineCurrent += delta;
				AmmoCurrent -= delta;
			}
			else
			{
				MagazineCurrent = AmmoCurrent;
				AmmoCurrent = 0;
			}
		}
	}

	ShootIntervalCurrent -= DeltaTime;
	
	BloomCurrent -= BloomDelta*DeltaTime;
	if(BloomCurrent < 0.0)
		BloomCurrent = 0.0;
}

