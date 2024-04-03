// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTask/Items/WeaponBase.h"

void AWeaponBase::Interact(APawn* Interactor)
{
}

bool AWeaponBase::StartPrimaryUse()
{ 
	UKismetSystemLibrary::PrintString(this, "StartPrimaryUse");
	return true;
}

bool AWeaponBase::StartSecondaryUse()
{
	UKismetSystemLibrary::PrintString(this, "StartSecondaryUse");
	return true;
}

void AWeaponBase::StopUse()
{
	UKismetSystemLibrary::PrintString(this, "StopUse");
}

void AWeaponBase::CancelUse()
{
	UKismetSystemLibrary::PrintString(this, "CancelUse");
}

void AWeaponBase::Shoot()
{
}

void AWeaponBase::LaunchProjectile()
{
}

void AWeaponBase::CalculateLineTrace()
{
}
