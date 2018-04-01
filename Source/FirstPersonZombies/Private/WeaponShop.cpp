// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponShop.h"


// Sets default values
AWeaponShop::AWeaponShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
}

// Called when the game starts or when spawned
void AWeaponShop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

