// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/SkeletalMesh.h"
#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum EWeaponAnimType
{
	Unarmed,
	Rifle,
	UZI,
	Pistol,
	Grenade
};

UENUM(BlueprintType)
enum EFireMode
{
	Single,
	Auto
};

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TEnumAsByte<EFireMode> FireMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
	int AmmoInMagazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
	int MaxAmmoInMagazine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
	int SpareAmmo;
};

/**
 * 
 */
UCLASS()
class TESTTASK_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	USkeletalMesh* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UAnimMontage* ShootingMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TEnumAsByte<EWeaponAnimType> WeaponAnimType;


	UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (ClampMin = 1))
	int Slot = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	FWeaponStats InitStats;
};
