// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestTask/Interfaces/InteractionInterface.h"
#include "Components/SphereComponent.h"
#include "PickupableActor.generated.h"

UENUM()
enum EItemState
{
	Dropped,
	Equipped
};

UCLASS()
class TESTTASK_API APickupableActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupableActor();

	UPROPERTY(VisibleAnywhere, Category = "Root")
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, Category = "Slots", meta = (ClampMin = 1))
	int Slot = 1;

	EItemState GetItemState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EItemState SetItemState(EItemState NewItemState, APawn* NewOwner);

private:
	
	EItemState ItemState = Dropped;

};
