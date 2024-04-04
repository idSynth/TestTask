// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "TestTask/Items/WeaponBase.h"
#include "Net/UnrealNetwork.h"
#include "WeaponInventoryComponent.generated.h"

class ATestTaskCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSwitched, AWeaponBase*, CurrentWeapon);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTTASK_API UWeaponInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponInventoryComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (ClampMin = 1))
	int Slots = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FName AttachSocketName = "WeaponAttach";


	// Equipping
	bool PickupClosestItem();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_PickupClosestItem();
	void Server_PickupClosestItem_Implementation() { PickupClosestItem(); }

	// Helping functions
	void EquipItem(AWeaponBase* Item);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Inventory")
	void Multicast_EquipItem(AWeaponBase* Item);
	void Multicast_EquipItem_Implementation(AWeaponBase* Item) { EquipItem(Item); }

	void DropItem(AWeaponBase* Item);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DropItem(AWeaponBase* Item);
	void Multicast_DropItem_Implementation(AWeaponBase* Item) { DropItem(Item); }

	//UFUNCTION(NetMulticast, Reliable)
	//void Multicast_SendAnimTypeToInstance(/*EWeaponAnimType will be here*/);
	//void Multicast_SendAnimTypeToInstance_Implementation(/*EWeaponAnimType will be here*/); /*{ Cast<IWeaponAnimInterface>(CurrentItem->WeaponMesh->GetAnimInstance())->SetWeaponAnimType(EWeaponAnimType); }*/

	// Inventory functions
	bool AddItemToInventory(AWeaponBase* PickupableActor);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_AddItemToInventory(AWeaponBase* Item);
	void Server_AddItemToInventory_Implementation(AWeaponBase* Item) { AddItemToInventory(Item); }

	bool RemoveItemFromInventory(AWeaponBase* Item);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_RemoveItemFromInventory(AWeaponBase* Item);
	void Server_RemoveItemFromInventory_Implementation(AWeaponBase* Item) { RemoveItemFromInventory(Item); }

	void RemoveAllItemsFromInventory();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_RemoveAllItemsFromInventory();
	void Server_RemoveAllItemsFromInventory_Implementation() { RemoveAllItemsFromInventory(); }

	bool RemoveItemBySlot(int Slot);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_RemoveItemBySlot(int Slot);
	void Server_RemoveItemBySlot_Implementation(int Slot) { RemoveItemBySlot(Slot); }

	const bool SelectItemInSlot(int Slot);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Inventory")
	void Server_SelectItemInSlot(int Slot);
	void Server_SelectItemInSlot_Implementation(int Slot) { SelectItemInSlot(Slot); }


	// Getters
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	const AWeaponBase* GetCurrentItem();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	const AWeaponBase* GetItemInSlot(int Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	const TArray<AWeaponBase*> GetAllItems();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponSwitched OnWeaponSwitched;

	//Replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	
	UPROPERTY(Replicated)
	TArray<AWeaponBase*> Items;

	UPROPERTY(Replicated)
	TObjectPtr<AWeaponBase> CurrentItem = nullptr;
};
