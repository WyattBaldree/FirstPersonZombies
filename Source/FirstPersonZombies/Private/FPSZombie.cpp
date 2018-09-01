// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSZombie.h"
#include "FirstPersonZombiesGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AFPSZombie::AFPSZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//SetRootComponent(Root);
	
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
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(ZombieManagerReference->ZombieList.Num()));
	}

	// This variable represents how long the zombie takes to spawn based on animation times.
	SpawnAnimationTime = SpawnAnimation->GetPlayLength()/SpawnAnimation->RateScale + StandAnimation->GetPlayLength()/StandAnimation->RateScale;

	AttackAnimationTime = AttackAnimation->GetPlayLength() / AttackAnimation->RateScale;

	ClimbOverAnimationTime = ClimbOverAnimation->GetPlayLength() / ClimbOverAnimation->RateScale;

	ZombieFeet = FVector(0, 0, 1) * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

// Called every frame
void AFPSZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UCharacterMovementComponent* MyCharacterMovement = GetCharacterMovement();
	AAIController* MyAIController = (AAIController*)GetController();
	
	
	// Upon entering a new state
	if (ZombieStateTarget != ZombieState) {

		switch (ZombieStateTarget) {
		case ZombieStateEnum::VE_Spawning:
			SpawnAnimationTime = SpawnAnimation->GetPlayLength() + StandAnimation->GetPlayLength();
			break;
		case ZombieStateEnum::VE_ToWindow:
			if (TargetWindow) {
				MyAIController->MoveToLocation(TargetWindow->EntryPoint->GetComponentLocation() + ZombieFeet);
			}
			break;
		case ZombieStateEnum::VE_AttackWindow:
			if (TargetWindow) {
				MyAIController->MoveToLocation(TargetWindow->AttackPoint->GetComponentLocation() + ZombieFeet);
			}
			AttackAnimationTime = AttackAnimation->GetPlayLength() / AttackAnimation->RateScale;
			hasAttacked = false;
			break;
		case ZombieStateEnum::VE_ClimbThroughWindow:
			ClimbOverAnimationTime = ClimbOverAnimation->GetPlayLength() / ClimbOverAnimation->RateScale;
			if (TargetWindow) {
				TargetWindow->Busy = true;
			}
			break;
		case ZombieStateEnum::VE_Dead:
			DeathTimerCurrent = DeathTimer;
			if (ZombieState == ZombieStateEnum::VE_ClimbThroughWindow) {
				if (TargetWindow) {
					SetActorLocation(TargetWindow->ExitPoint->GetComponentLocation() + ZombieFeet);
					TargetWindow->Busy = false;
				}
			}
			break;
		case ZombieStateEnum::VE_Walking:
			if (TargetActor) {
				MyAIController->MoveToActor(TargetActor);
			}
			break;
		case ZombieStateEnum::VE_Running:
			if (TargetActor) {
				MyAIController->MoveToActor(TargetActor);
			}
			break;
		case ZombieStateEnum::VE_Crawling:
			FallAnimationTime = FallAnimation->GetPlayLength();
			if (TargetActor) {
				MyAIController->MoveToActor(TargetActor);
			}
			break;
		}

		ZombieState = ZombieStateTarget;
	}

	switch (ZombieState) {
	case ZombieStateEnum::VE_Spawning:
		MyCharacterMovement->MaxWalkSpeed = 0.0f;
		MyCharacterMovement->RotationRate.Yaw = 0.0f;
		SetActorEnableCollision(true);

		SpawnAnimationTime -= DeltaTime;

		if (SpawnAnimationTime <= 0) {
			ZombieStateTarget = MovementStatePreference;
		}
		break;
	case ZombieStateEnum::VE_Walking:
		MyCharacterMovement->MaxWalkSpeed = ZombieWalkSpeed;
		MyCharacterMovement->RotationRate.Yaw = 120.0f;
		SetActorEnableCollision(true);
		if (TargetActor) {
			MyAIController->MoveToActor(TargetActor);
		}
		break;
	case ZombieStateEnum::VE_ClimbThroughWindow:
		MyCharacterMovement->MaxWalkSpeed = ZombieWalkSpeed;
		MyCharacterMovement->RotationRate.Yaw = 0.0f;
		SetActorEnableCollision(true);

		if (TargetWindow) {

			if (ClimbOverAnimationTime > 0 && HP >= 0) {
				ClimbOverAnimationTime -= DeltaTime;

		
				FVector AnimationPointVector = TargetWindow->EntryPoint->GetComponentLocation() + ZombieFeet - GetActorLocation();
				float SnapSpeed = 10.0f * DeltaTime;
				AnimationPointVector.X -= (FMath::Abs(AnimationPointVector.X) < SnapSpeed) ? AnimationPointVector.X : FMath::Sign(AnimationPointVector.X) * SnapSpeed;
				AnimationPointVector.Y -= (FMath::Abs(AnimationPointVector.Y) < SnapSpeed) ? AnimationPointVector.Y : FMath::Sign(AnimationPointVector.Y) * SnapSpeed;
				AnimationPointVector.Z -= (FMath::Abs(AnimationPointVector.Z) < SnapSpeed) ? AnimationPointVector.Z : FMath::Sign(AnimationPointVector.Z) * SnapSpeed;

				SetActorLocation(GetActorLocation() + AnimationPointVector);
				SetActorRotation(TargetWindow->GetActorRotation());
				
			}
			else if (SetActorLocation(TargetWindow->ExitPoint->GetComponentLocation() + ZombieFeet)) {
				ZombieStateTarget = MovementStatePreference;
				TargetWindow->Busy = false;
			}
		}


		break;
	case ZombieStateEnum::VE_ToWindow:
		MyCharacterMovement->MaxWalkSpeed = ZombieWalkSpeed;
		MyCharacterMovement->RotationRate.Yaw = 120.0f;
		SetActorEnableCollision(true);

		if (TargetWindow) {
			MyAIController->MoveToLocation(TargetWindow->EntryPoint->GetComponentLocation() + ZombieFeet);

			if (HP >= 0 && FVector::Dist(TargetWindow->EntryPoint->GetComponentLocation(), GetActorLocation() - ZombieFeet) < 50.0f) {

				if (TargetWindow->NumBoards > 0) {
					ZombieStateTarget = ZombieStateEnum::VE_AttackWindow;

				}
				else if (TargetWindow->Busy == false) {
					ZombieStateTarget = ZombieStateEnum::VE_ClimbThroughWindow;
				}
			}
		}
		break;
	case ZombieStateEnum::VE_AttackWindow:
		MyCharacterMovement->MaxWalkSpeed = 0.0f;
		MyCharacterMovement->RotationRate.Yaw = 0.0f;
		SetActorEnableCollision(true);
		MyAIController->MoveToLocation(TargetWindow->AttackPoint->GetComponentLocation() + ZombieFeet);

		if (AttackAnimationTime > 0 && HP >= 0) {
			AttackAnimationTime -= DeltaTime;
			if (AttackAnimationTime <= 0) {
				ZombieStateTarget = ZombieStateEnum::VE_ToWindow;
			}
			else if (AttackAnimationTime <= WindowHitDelay && hasAttacked == false) {
				if (TargetWindow) {
					if (TargetWindow->NumBoards > 0) {
						TargetWindow->NumBoards = TargetWindow->NumBoards - 1;
					}
				}
				hasAttacked = true;
			}
		}
		break;
	case ZombieStateEnum::VE_Running:
		MyCharacterMovement->MaxWalkSpeed = ZombieRunSpeed;
		MyCharacterMovement->RotationRate.Yaw = 120.0f;
		SetActorEnableCollision(true);
		if (TargetActor) {
			MyAIController->MoveToActor(TargetActor);
		}
		break;
	case ZombieStateEnum::VE_Crawling:
		MyCharacterMovement->MaxWalkSpeed = ZombieCrawlSpeed;
		MyCharacterMovement->RotationRate.Yaw = 60.0f;
		SetActorEnableCollision(true);
		if (TargetActor) {
			MyAIController->MoveToActor(TargetActor);
		}

		if (FallAnimationTime > 0) {
			MyCharacterMovement->MaxWalkSpeed = 0.0f;
			FallAnimationTime -= DeltaTime;
		}
		else {
			MyCharacterMovement->MaxWalkSpeed = ZombieCrawlSpeed;
		}
		break;
	case ZombieStateEnum::VE_Dead:
		MyCharacterMovement->MaxWalkSpeed = 0.0f;
		MyCharacterMovement->RotationRate.Yaw = 0.0f;
		SetActorEnableCollision(false);

		DeathTimerCurrent -= DeltaTime;
		if (DeathTimerCurrent <= 0) {
			Die();
		}
		break;
	}

		if (Attacking) {
			MyCharacterMovement->MaxWalkSpeed = 0.0f;
		}

}

// Called to bind functionality to input
void AFPSZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFPSZombie::ClimbOver(float DeltaTime)
{
	ClimbOverAnimationTime -= DeltaTime;

	ZombieStateTarget = ZombieStateEnum::VE_Walking;
}

void AFPSZombie::AttackWindow(float DeltaTime)
{
	AttackAnimationTime -= DeltaTime;

	ZombieStateTarget = ZombieStateEnum::VE_ToWindow;
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

