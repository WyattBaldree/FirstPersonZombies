// Fill out your copyright notice in the Description page of Project Settings.

#include "DummyWeaponShop.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/ArrowComponent.h"


// Sets default values
ADummyWeaponShop::ADummyWeaponShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UObject* Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
}

// Called when the game starts or when spawned
void ADummyWeaponShop::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World)
	{
		TSubclassOf<class AWeaponShop> SpawnWeapon = WeaponShopClasses[FMath::RandHelper(WeaponShopClasses.Num())];

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		//Spawn the projectile at the muzzle
		World->SpawnActor<AWeaponShop>(SpawnWeapon, GetActorLocation(), GetActorRotation(), SpawnParams);
	}

	Destroy();
}

// Called every frame
void ADummyWeaponShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

