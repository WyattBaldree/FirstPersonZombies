// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimSequence.h"
#include "Window.h"
#include "FPSZombie.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ZombieStateEnum : uint8
{
	VE_Spawning 	UMETA(DisplayName = "Spawning"),
	VE_ToWindow		UMETA(DisplayName = "ToWindow"),
	VE_AttackWindow		UMETA(DisplayName = "AttackWindow"),
	VE_ClimbThroughWindow		UMETA(DisplayName = "ClimbThroughWindow"),
	VE_Walking 	UMETA(DisplayName = "Walking"),
	VE_Running 	UMETA(DisplayName = "Running"),
	VE_Dead		UMETA(DisplayName = "Dead"),
	VE_Crawling		UMETA(DisplayName = "Crawling")
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

	// This function behaves as an event. If this event has not been used in a blueprint, it will perform the EventName_Implementation code in the cpp file. 
	// EventName_Implementation cannot be overridden, so in order to have different functionality in child classes, you must instead create and override an overridable function which is called by Interact_Implementation.
	// If this event has been used in a blueprint, the actor will completely ignore the Interact_Implementation code and use the blueprint functionality instead.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void IdleNoiseEvent(); // The name of our Event in unreal. The const parameter value appears as an input on the blueprint event.
	void IdleNoiseEvent_Implementation(); // This must be declared in order for the Interact_Implementation to work properly

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void HurtNoiseEvent(); // The name of our Event in unreal. The const parameter value appears as an input on the blueprint event.
	void HurtNoiseEvent_Implementation(); // This must be declared in order for the Interact_Implementation to work properly 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void DieNoiseEvent(); // The name of our Event in unreal. The const parameter value appears as an input on the blueprint event.
	void DieNoiseEvent_Implementation(); // This must be declared in order for the Interact_Implementation to work properly

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gameplay")
	void AttackNoiseEvent(); // The name of our Event in unreal. The const parameter value appears as an input on the blueprint event.
	void AttackNoiseEvent_Implementation(); // This must be declared in order for the Interact_Implementation to work properly

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

	/** This is the percentage of damamge that must be done to the zombies legs in order to cause them to crawl. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float CrippleRatio = .75;

	/** This is the amount of damage that has been done to the zombies legs. */
	float CrippleDamage = 0.0;

	/** This is the percentage chance that a zombie will begin running when you shoot him. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float WalkToRunChance = .50;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	AWindow* TargetWindow;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Zombie")
	bool Hurt(float Damage, bool Headshot);
	bool Hurt_Implementation(float Damage, bool Headshot);

	UFUNCTION(Category = "Zombie")
	bool Die();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Zombie")
	void DieEvent();
	void DieEvent_Implementation();

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

	/** AnimMontage to play when we crawl */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimSequence* AttackAnimation;

	/** AnimMontage to play when we crawl */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	UAnimSequence* ClimbOverAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float SpawnAnimationTime;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FallAnimationTime;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float AttackAnimationTime;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float ClimbOverAnimationTime;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float DeathTimer = 5;

	float DeathTimerCurrent = DeathTimer;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
	bool Attacking = false;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float WindowHitDelay = 1.0;

	void ClimbOver(float DeltaTime);

	void AttackWindow(float DeltaTime);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "_Window")
	class USceneComponent* Root;
	
	ZombieStateEnum MovementStatePreference = ZombieStateEnum::VE_Walking;
private:
	AZombieManager* ZombieManagerReference;

	bool hasAttacked = false;

	FVector ZombieFeet;

};
