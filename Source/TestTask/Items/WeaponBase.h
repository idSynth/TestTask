// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestTask/Items/PickupableActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WeaponBase.generated.h"



/**
 * 
 */
UCLASS()
class TESTTASK_API AWeaponBase : public APickupableActor
{
	GENERATED_BODY()
	
public:
	
	// Interface implementation starts 
	
	void Interact(APawn* Interactor) override;

	bool StartPrimaryUse() override;

	bool StartSecondaryUse() override;

	void StopUse() override;

	void CancelUse() override;

	// Interface implementation ends


	void Shoot();

	void LaunchProjectile();

	void CalculateLineTrace();

	// TODO: Recoil
	void AddRecoil();
};
