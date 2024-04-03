// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTask/Components/WeaponInventoryComponent.h"
#include "TestTask/Character/TestTaskCharacter.h"

// Sets default values for this component's properties
UWeaponInventoryComponent::UWeaponInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}

// TODO: cleanup this function (maybe)?
bool UWeaponInventoryComponent::PickupClosestItem()
{
	float Distance;

	TArray<AActor*> OverlappingActors;
	GetOwner()->GetOverlappingActors(OverlappingActors, AWeaponBase::StaticClass());
	
	if (OverlappingActors.IsEmpty())
	{
		return false;
	}

	AWeaponBase* NearestActor = Cast<AWeaponBase>(UGameplayStatics::FindNearestActor(GetOwner()->GetActorLocation(), OverlappingActors, Distance));
	AddItemToInventory(NearestActor);

	return true;
}


void UWeaponInventoryComponent::EquipItem(AWeaponBase* Item)
{
	auto* Owner = Cast<ACharacter>(GetOwner());

	Item->SetItemState(EItemState::Equipped, Owner);
	Item->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocketName);
}

void UWeaponInventoryComponent::DropItem(AWeaponBase* Item)
{
	Item->SetItemState(EItemState::Dropped, nullptr);
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}



bool UWeaponInventoryComponent::AddItemToInventory(AWeaponBase* Item)
{
	Items.Insert(Item, 0);
	EquipItem(Item);
	SelectItemInSlot(0);

	return true;
}

bool UWeaponInventoryComponent::RemoveItemFromInventory(AWeaponBase* Item)
{
	if (!IsValid(Item))
	{
		return false;
	}

	Item->CancelUse();
	DropItem(Item);
	Items.Remove(Item);

	return true;
}

bool UWeaponInventoryComponent::RemoveItemBySlot(int Slot)
{
	
	if (!Items.IsValidIndex(Slot))
	{
		return false;
	}

	if (!IsValid(Items[Slot]))
	{
		return false;
	}

	return RemoveItemFromInventory(Items[Slot]);;
}

const bool UWeaponInventoryComponent::SelectItemInSlot(int Slot)
{
	if (!Items.IsValidIndex(Slot))
	{
		return false;
	}

	if (!IsValid(Items[Slot]))
	{
		return false;
	}

	if (IsValid(CurrentItem))
	{
		CurrentItem->CancelUse();
	}

	CurrentItem = Items[Slot];
	return true;
}

const AWeaponBase* UWeaponInventoryComponent::GetCurrentItem()
{
	return CurrentItem;
}

const AWeaponBase* UWeaponInventoryComponent::GetItemInSlot(int Slot)
{
	if (!Items.IsValidIndex(Slot))
	{
		return nullptr;
	}

	return Items[Slot];
}

const TArray<AWeaponBase*> UWeaponInventoryComponent::GetAllItems()
{
	return Items;
}


// Called when the game starts
void UWeaponInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

