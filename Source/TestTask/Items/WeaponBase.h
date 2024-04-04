// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestTask/Items/PickupableActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponBase.generated.h"

class ACharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class TESTTASK_API AWeaponBase : public APickupableActor
{
	GENERATED_BODY()
	
public:
	
	AWeaponBase();

	UPROPERTY(VisibleAnywhere, Category = "Visuals")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName MuzzleSocketName = "Muzzle";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TEnumAsByte<EDrawDebugTrace::Type> TraceDebug;

	// Interface implementation starts 
	
	void Interact(APawn* Interactor) override;

	bool StartPrimaryUse() override;

	bool StartSecondaryUse() override;

	void StopUse() override;

	void CancelUse() override;

	// Interface implementation ends

	void ToggleAim(bool isAiming);

	void Shoot();

	void LaunchProjectile();

	void CalculateLineTrace();

	// TODO: Recoil
	void AddRecoil();
};
