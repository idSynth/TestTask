// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTask/Items/PickupableActor.h"

// Sets default values
APickupableActor::APickupableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Collision->SetSphereRadius(50);
	Collision->SetCollisionProfileName("DroppedItem");
	Collision->BodyInstance.bLockXRotation = true;
	Collision->BodyInstance.bLockYRotation = true;
}

EItemState APickupableActor::GetItemState()
{
	return ItemState;
}

// Called when the game starts or when spawned
void APickupableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EItemState APickupableActor::SetItemState(EItemState NewItemState, APawn* NewOwner)
{
	ItemState = NewItemState;
	switch (ItemState)
	{
	case Dropped:
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Collision->SetSimulatePhysics(true);
		SetOwner(NewOwner);
		SetInstigator(NewOwner);
		break;
	case Equipped:
		Collision->SetSimulatePhysics(false);
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetOwner(NewOwner);
		SetInstigator(NewOwner);
		break;
	}
	return ItemState;
}

