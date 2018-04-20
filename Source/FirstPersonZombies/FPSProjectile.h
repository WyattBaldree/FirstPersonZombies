// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class FIRSTPERSONZOMBIES_API AFPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Sphere collision component
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComponent;

	// Projectile Movement Component
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireInDirection(const FVector& ShootDirection);

	//Function that is called when we hit something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	// sweg
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	AActor* hitlist[10];
	int hitlength = 0;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Damage = 12.5f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	int Pierce = 3;

	/** Sound to play each time we hit a zombie */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	USoundBase* HitMarkerSound;

	/** Particle to spawn when we shoot something */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UParticleSystem* HitParticle;

	/** Particle to spawn when we shoot a zombie */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	UParticleSystem* BloodParticle;
};
