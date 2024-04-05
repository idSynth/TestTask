// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TestTask/Items/Data/WeaponData.h"
#include "WeaponAnimInterface.generated.h"

UINTERFACE(BlueprintType)
class UWeaponAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TESTTASK_API IWeaponAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "WeaponAnimation")
	void SetWeaponAnimType(EWeaponAnimType NewWeaponAnimType);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "WeaponAnimation")
	void SetAiming(bool bIsAiming);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "WeaponAnimation")
	void PlayShootingMontage();
};
