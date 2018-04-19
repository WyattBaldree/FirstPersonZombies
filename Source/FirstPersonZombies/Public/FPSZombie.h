// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimSequence.h"
#include "FPSZombie.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ZombieStateEnum : uint8
{
	VE_Spawning 	UMETA(DisplayName = "Spawning"),
	VE_Walking 	UMETA(DisplayName = "Walking"),
	VE_Running 	UMETA(DisplayName = "Running"),
	VE_Crawling		UMETA(DisplayName = "Crawling"),
	VE_Dead		UMETA(DisplayName = "Dead")
};

class AZombieManager;

UCLASS()
class FIRSTPERSONZOMBIES_API AFPSZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float MaxHP = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float HP = MaxHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float ZombieRunSpeed = 300.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float ZombieWalkSpeed = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float ZombieCrawlSpeed = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float CurrentDelta = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	AActor* TargetActor = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float UpdateInterval = 0.1f;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	ZombieStateEnum ZombieState = ZombieStateEnum::VE_Spawning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	ZombieStateEnum ZombieStateTarget = ZombieState;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Zombie")
	bool Hurt(float Damage, bool Headshot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Zombie")
	bool Die();

	/** AnimMontage to play when we spawn */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimSequence* SpawnAnimation;

	/** AnimMontage to play when we finish spawning or get up from crawling */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimSequence* StandAnimation;

	/** AnimMontage to play when we run */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimSequence* RunAnimation;

	/** AnimMontage to play when we walk */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimSequence* WalkAnimation;

	/** AnimMontage to play when we idle */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimSequence* IdleAnimation;

	/** AnimMontage to play when we crawl */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimSequence* FallAnimation;

	/** AnimMontage to play when we crawl */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimSequence* DieAnimation;

	/** AnimMontage to play when we crawl */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimSequence* CrawlAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float SpawnAnimationTime;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FallAnimationTime;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float DeathTimer = 5;

	float DeathTimerCurrent = DeathTimer;
	
private:
	AZombieManager* ZombieManagerReference;


};
