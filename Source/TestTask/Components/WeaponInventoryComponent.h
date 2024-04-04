// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "TestTask/Items/WeaponBase.h"
#include "WeaponInventoryComponent.generated.h"

class ATestTaskCharacter;

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
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool PickupClosestItem();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EquipItem(AWeaponBase* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropItem(AWeaponBase* Item);

	// Inventory functions
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItemToInventory(AWeaponBase* PickupableActor);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemFromInventory(AWeaponBase* PickupableActor);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveAllItemsFromInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemBySlot(int Slot);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const bool SelectItemInSlot(int Slot);


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

private:
	
	TArray<AWeaponBase*> Items;
	TObjectPtr<AWeaponBase> CurrentItem = nullptr;
};
