// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTask/Components/WeaponInventoryComponent.h"
#include "TestTask/Character/TestTaskCharacter.h"

// Sets default values for this component's properties
UWeaponInventoryComponent::UWeaponInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

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
	Item->SetActorHiddenInGame(false);
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Item->SetActorRotation(FQuat::MakeFromRotator(FRotator(0, Item->GetActorRotation().Yaw, 0)));
	Item->SetItemState(EItemState::Dropped, nullptr);
}


void UWeaponInventoryComponent::Multicast_SendAnimTypeToInstance_Implementation(EWeaponAnimType WeaponAnimType)
{
	UAnimInstance* OwnerAnimInstance = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();
	if (!IsValid(OwnerAnimInstance))
	{
		return;
	}
	Cast<IWeaponAnimInterface>(OwnerAnimInstance)->Execute_SetWeaponAnimType(OwnerAnimInstance, WeaponAnimType);
}

bool UWeaponInventoryComponent::AddItemToInventory(AWeaponBase* Item)
{
	if (!Items.IsValidIndex(Item->Slot))
	{
		return false;
	}

	if (IsValid(Items[Item->Slot]))
	{
		RemoveItemFromInventory(Items[Item->Slot]);
	}

	Items[Item->Slot] = Item;
	Multicast_EquipItem(Item);
	SelectItemInSlot(Item->Slot);

	return true;
}

bool UWeaponInventoryComponent::RemoveItemFromInventory(AWeaponBase* Item)
{
	if (!IsValid(Item))
	{
		return false;
	}

	Item->CancelUse();
	Multicast_DropItem(Item);
	if (CurrentItem == Item)
	{
		CurrentItem = nullptr;
	}

	Items[Item->Slot] = nullptr;
	SelectItemInSlot(0);

	return true;
}

void UWeaponInventoryComponent::RemoveAllItemsFromInventory()
{
	for (const auto& Item : Items)
	{
		RemoveItemFromInventory(Item);
	}
}

bool UWeaponInventoryComponent::RemoveItemBySlot(int Slot)
{
	if (!Items.IsValidIndex(Slot))
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

	for (const auto& Item : Items)
	{
		if (IsValid(Item))
		{
			Item->SetActorHiddenInGame(true);
		}
	}

	if (IsValid(CurrentItem))
	{
		CurrentItem->CancelUse();
	}

	if (!IsValid(Items[Slot]))
	{
		Multicast_SendAnimTypeToInstance(EWeaponAnimType::Unarmed);
		CurrentItem = nullptr;
		Multicast_BroadcastWeaponSwitch(nullptr);
		return false;
	}

	CurrentItem = Items[Slot];
	CurrentItem->SetActorHiddenInGame(false);
	Multicast_BroadcastWeaponSwitch(Items[Slot]);
	Multicast_SendAnimTypeToInstance(CurrentItem->WeaponData->WeaponAnimType);
	return true;
}

void UWeaponInventoryComponent::BroadcastWeaponSwitch(AWeaponBase* Item)
{
	OnWeaponSwitched.Broadcast(CurrentItem);
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
	
	//Init slots and add 1 to leave the first slot for empty hands
	Items.SetNum(Slots+1);
}


// Called every frame
void UWeaponInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponInventoryComponent, CurrentItem);
	DOREPLIFETIME(UWeaponInventoryComponent, Items);
}

