// Fill out your copyright notice in the Description page of Project Settings.

#include "RayTracing.h"
constexpr ECollisionChannel ECC_Projectile = ECollisionChannel::ECC_GameTraceChannel1;
RayTracing::RayTracing()
{
}

RayTracing::~RayTracing()
{
}

bool RayTracing::RayCast(AActor* caller, FVector location, FRotator direction, float distance, FHitResult* hit, FCollisionQueryParams* params)
{
	FVector start = location;
	FVector end = location + (direction.Vector() * distance);

	params->bTraceComplex = true;
	params->bTraceAsyncScene = false;//true maybe?

	UWorld * hey = caller->GetWorld();

	bool Traced = hey->LineTraceSingleByChannel(
		*hit, start, end, ECC_Projectile, *params
	);

	return Traced;
}
