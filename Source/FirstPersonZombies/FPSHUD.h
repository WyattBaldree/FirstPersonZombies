// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

class UTexture2D;
/**
 * 
 */
UCLASS()
class FIRSTPERSONZOMBIES_API AFPSHUD : public AHUD
{
	GENERATED_BODY()

	protected:
		// this is what we're
		UPROPERTY(EditDefaultsOnly)
		UTexture2D* CrosshairTexture;
	
	public:
		//Primary draw call for the HUD.
		virtual void DrawHUD() override;
	
};
