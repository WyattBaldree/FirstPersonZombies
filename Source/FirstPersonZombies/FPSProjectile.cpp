// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProjectile.h"
#include "FPSZombie.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Die after 3 seconds
	InitialLifeSpan = 3.0f;

	// Use a sphere as a simple collision representation.
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("FPSProjectile"));
	CollisionComponent->SetNotifyRigidBodyCollision(true);
	//You only need to register the OnHit function
	CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnCompHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSProjectile::OnOverlapBegin);	// set up a notification for when this component hits something blocking
	// Set the sphere's collision radius.
	CollisionComponent->InitSphereRadius(15.0f);
	// Set the root component to be the collision component.
	RootComponent = CollisionComponent;

	

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 9000.0f;
	ProjectileMovementComponent->MaxSpeed = 9000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSProjectile::FireInDirection(const FVector & ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

//function that is called when the projectile hits something
void AFPSProjectile::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComponent != NULL) && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, GetActorLocation());

		
	}
	FVector SpawnLocation = Hit.ImpactPoint;
	FRotator SpawnRotation = Hit.ImpactNormal.Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, SpawnLocation, SpawnRotation, true);

	Destroy();
}

void AFPSProjectile::ReceiveHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	FVector SpawnLocation = Hit.ImpactPoint;
	FRotator SpawnRotation = Hit.ImpactNormal.Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, SpawnLocation, SpawnRotation, true);

	Destroy();
}

/*void AFPSProjectile::ReceiveHit(UPrimitiveComponent * MyComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	FVector SpawnLocation = Hit.ImpactPoint;
	FRotator SpawnRotation = Hit.ImpactNormal.Rotation();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, SpawnLocation, SpawnRotation, true);

	Destroy();
}*/

void AFPSProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	
	
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		int i = 0;
		for (i = 0; i < hitlength; i++) {
			if (hitlist[i] == OtherActor) {
				return;
			}
		}
		if (hitlength < 20) {
			hitlist[hitlength] = OtherActor;
			hitlength++;
		}

		if (OtherActor->IsA(AFPSZombie::StaticClass())) {
			AFPSZombie* zombie = (AFPSZombie*)OtherActor;

			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, SweepResult.BoneName.ToString());
			}

			bool headhit = false;
			if (SweepResult.BoneName == FName(TEXT("Head")))
			{
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Headshot!"));
				}
				headhit = true;
			}
			else if (SweepResult.BoneName == FName(TEXT("RightFoot")) || 
					 SweepResult.BoneName == FName(TEXT("RightLeg")) ||	
					 SweepResult.BoneName == FName(TEXT("RightUpLeg")) || 
					 SweepResult.BoneName == FName(TEXT("LeftFoot")) || 
					 SweepResult.BoneName == FName(TEXT("LeftLeg")) || 
					 SweepResult.BoneName == FName(TEXT("LeftUpLeg"))) {
				zombie->CrippleDamage += Damage;
			}
			zombie->Hurt(Damage, headhit);
			if (headhit) {
				if (HeadHitMarkerSound) {
					UGameplayStatics::PlaySound2D(this, HeadHitMarkerSound, 1.0f, FMath::RandRange(1.0f - PitchRandomness, 1.0f + PitchRandomness));
				}
			}
			if (HitMarkerSound) {
				UGameplayStatics::PlaySound2D(this, HitMarkerSound, 1.0f, FMath::RandRange(1.0f - PitchRandomness, 1.0f + PitchRandomness));
			}

			//"FRotator rotPelvis = Mesh->MeshGetInstance(this))->GetBoneRotation(FName(TEXT("pelvis")));"
			FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
			FVector SpawnLocation = SweepResult.ImpactPoint + (CameraLocation - SweepResult.ImpactPoint).GetSafeNormal(0.000001) * 40.0;
			FRotator SpawnRotation = SweepResult.ImpactNormal.Rotation();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodParticle, SpawnLocation, SpawnRotation, true);
		}
	}
	if (hitlength >= Pierce) Destroy();
}
