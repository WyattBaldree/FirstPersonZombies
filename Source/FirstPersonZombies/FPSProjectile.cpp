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
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSProjectile::OnOverlapBegin);	// set up a notification for when this component hits something blocking
	// Set the sphere's collision radius.
	CollisionComponent->InitSphereRadius(15.0f);
	// Set the root component to be the collision component.
	RootComponent = CollisionComponent;

	

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
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
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComponent != NULL) && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, GetActorLocation());

		
	}

	//Destroy();
}

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

			if (HitMarkerSound) {
				UGameplayStatics::PlaySoundAtLocation(this, HitMarkerSound, UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
			}

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
			zombie->Hurt(Damage, headhit);

			//"FRotator rotPelvis = Mesh->MeshGetInstance(this))->GetBoneRotation(FName(TEXT("pelvis")));"
		}
	}
	if (hitlength >= Pierce) Destroy();
}
