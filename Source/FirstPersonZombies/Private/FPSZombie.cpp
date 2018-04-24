// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSZombie.h"
#include "FirstPersonZombiesGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AFPSZombie::AFPSZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AFPSZombie::BeginPlay()
{
	Super::BeginPlay();

	//Randomize the update interval so not all zombies are updating at the same time.
	CurrentDelta = FMath::RandRange(0.0f, UpdateInterval);

	//Set the character movement speed.
	UCharacterMovementComponent* MyCharacterMovement = GetCharacterMovement();
	MyCharacterMovement->MaxWalkSpeed = 0.0;
	MyCharacterMovement->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	
	//When this Zombie is created, add it to the ZombieList in our ZombieManager.
	AFirstPersonZombiesGameMode* GameMode = (AFirstPersonZombiesGameMode*)GetWorld()->GetAuthGameMode();
	ZombieManagerReference = GameMode->MyZombieManager;

	//if we don't already have a Zombie manager, go ahead and create one.
	if (ZombieManagerReference == NULL) {
		GameMode->MakeZombieManager();
		ZombieManagerReference = GameMode->MyZombieManager;
	}
	//Add the zombie to the ZombieList.
	ZombieManagerReference->ZombieList.Add(this);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(ZombieManagerReference->ZombieList.Num()));
	}

	// This variable represents how long the zombie takes to spawn based on animation times.
	SpawnAnimationTime = SpawnAnimation->GetPlayLength() + StandAnimation->GetPlayLength();
}

// Called every frame
void AFPSZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	UCharacterMovementComponent* MyCharacterMovement = GetCharacterMovement();
	
	if (ZombieState == ZombieStateEnum::VE_Spawning && SpawnAnimationTime > 0 && HP >= 0) {
		SpawnAnimationTime -= DeltaTime;
	}
	else {
		if (ZombieState == ZombieStateEnum::VE_Dead) {
			DeathTimerCurrent -= DeltaTime;
			if (DeathTimerCurrent <= 0) {
				Die();
			}
		} else if (ZombieStateTarget != ZombieState) {

			ZombieState = ZombieStateTarget;

			switch (ZombieStateTarget) {
				case ZombieStateEnum::VE_Spawning:
					SpawnAnimationTime = SpawnAnimation->GetPlayLength() + StandAnimation->GetPlayLength();
					MyCharacterMovement->MaxWalkSpeed = 0.0f;
					MyCharacterMovement->RotationRate.Yaw = 0.0f;
					SetActorEnableCollision(true);
					break;
				case ZombieStateEnum::VE_Walking:
					MyCharacterMovement->MaxWalkSpeed = ZombieWalkSpeed;
					MyCharacterMovement->RotationRate.Yaw = 120.0f;
					SetActorEnableCollision(true);
					break;
				case ZombieStateEnum::VE_Running:
					MyCharacterMovement->MaxWalkSpeed = ZombieRunSpeed;
					MyCharacterMovement->RotationRate.Yaw = 120.0f;
					SetActorEnableCollision(true);
					break;
				case ZombieStateEnum::VE_Crawling:
					MyCharacterMovement->MaxWalkSpeed = ZombieCrawlSpeed;
					FallAnimationTime = FallAnimation->GetPlayLength();
					MyCharacterMovement->RotationRate.Yaw = 60.0f;
					SetActorEnableCollision(true);
					break;
				case ZombieStateEnum::VE_Dead:
					MyCharacterMovement->MaxWalkSpeed = 0.0f;
					MyCharacterMovement->RotationRate.Yaw = 0.0f;
					DeathTimerCurrent = DeathTimer;
					SetActorEnableCollision(false);
					break;
			}
		}else if (ZombieState == ZombieStateEnum::VE_Crawling) {
			if (FallAnimationTime > 0) {
				MyCharacterMovement->MaxWalkSpeed = 0.0f;
				FallAnimationTime -= DeltaTime;
			}
			else {
				MyCharacterMovement->MaxWalkSpeed = ZombieCrawlSpeed;
			}
		}
	}
}

// Called to bind functionality to input
void AFPSZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


bool AFPSZombie::Hurt_Implementation(float Damage, bool Headshot)
{
	if (Headshot) {
		HP -= Damage*1.9;
	}else{
		HP -= Damage;
	}

	if (HP <= 0) 
	{
		ZombieStateTarget = ZombieStateEnum::VE_Dead;
		return true;
	}

	if (ZombieState == ZombieStateEnum::VE_Walking && FMath::RandRange(0.0f, 1.0f) > WalkToRunChance) {
		ZombieStateTarget = ZombieStateEnum::VE_Running;
	}

	if ((ZombieState == ZombieStateEnum::VE_Running || ZombieState == ZombieStateEnum::VE_Walking) && CrippleDamage > CrippleRatio*MaxHP) {
		ZombieStateTarget = ZombieStateEnum::VE_Crawling;
	}

	return false;
}

bool AFPSZombie::Die_Implementation()
{
	ZombieManagerReference->ZombieList.Remove(this);
	ZombieManagerReference->CurrentWaveCount--;
	Destroy();
	return false;
}

