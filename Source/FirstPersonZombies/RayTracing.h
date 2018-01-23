// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Public/CollisionQueryParams.h"
#include "Runtime/Engine/Classes/Engine/World.h"

/**
 * 
 */
class FIRSTPERSONZOMBIES_API RayTracing
{
public:
	RayTracing();
	~RayTracing();

	bool RayCast(AActor* caller, FVector start, FRotator direction, float distance, FHitResult * RV_Hit, FCollisionQueryParams * RV_TraceParams);

	
};
