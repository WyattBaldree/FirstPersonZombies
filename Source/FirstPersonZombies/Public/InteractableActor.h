// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "InteractableActor.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EInteractableTypeEnum : uint8
{
	VE_Proximity 	UMETA(DisplayName = "Proximity"),
	VE_Button	UMETA(DisplayName = "Button"),
};

//The purpose of this call is to be extended by other blueprints or classes.

UCLASS()
class FIRSTPERSONZOMBIES_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// This function behaves as an event. If this event has not been used in a blueprint, it will perform the Interact_Implementation code in InteractableActor.cpp. 
	// Interact_Implementation cannot bee overridden so in order to have different functionality, you must instead override InteractionInternal which is called by Interact_Implementation.
	// If this event has been used in a blueprint, the actor will completely ignore the Interact_Implementation code and use the blueprint functionality instead.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "A_Interactable")
		void Interact();
		void Interact_Implementation(); // This must be declared in order for the Interact_Implementation to work properly 
	
	// This event will trigger when the interactable object overlaps with an actor
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "A_Interactable")
		void InteractOverlap();
		void InteractOverlap_Implementation();

	// This event will trigger when the interactable object begins overlaping an actor
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "A_Interactable")
		void InteractBegin();
		void InteractBegin_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "A_Interactable")
		void InteractEnd();
		void InteractEnd_Implementation();

	// This is the text that the user will see when the interactable actor is available for interaction.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Interactable")
		FString InteractableString = "Default interactable string...";
	
	// The distance at which the object can be interacted with
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Interactable")
		float InteractableDistance = 50;
	// If an interactable object has higher priority, the player will be more likely to be able to interact with it.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Interactable")
		float InteractablePriority = 5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Interactable")
	EInteractableTypeEnum InteractableType = EInteractableTypeEnum::VE_Proximity;

	UFUNCTION(BlueprintCallable, Category = "A_Interactable")
	void SetInteractString(FString NewString);

	UFUNCTION(BlueprintCallable, Category = "A_Interactable")
	FString GetInteractString();
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// This function allows us to give child classes different functionality when the Interact_Implementation is called since Interact_Implementation cannot be virtual.
	virtual void InteractInternal();

	virtual void InteractOverlapInternal();

	virtual void InteractBeginInternal();

	virtual void InteractEndInternal();


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "A_Interactable")
		bool Interacting = false;

	// sweg
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "A_Interactable")
		class UBoxComponent* BoxComponent;
	
};
