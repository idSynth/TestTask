// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTask/Items/WeaponBase.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


AWeaponBase::AWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(Collision);
}

void AWeaponBase::Interact(APawn* Interactor)
{
}

bool AWeaponBase::StartPrimaryUse()
{ 
	Shoot();
	UKismetSystemLibrary::PrintString(this, "StartPrimaryUse");
	return true;
}

bool AWeaponBase::StartSecondaryUse()
{
	ToggleAim(true);
	UKismetSystemLibrary::PrintString(this, "StartSecondaryUse");
	return true;
}

void AWeaponBase::StopUse()
{
	ToggleAim(false);
	UKismetSystemLibrary::PrintString(this, "StopUse");
}

void AWeaponBase::CancelUse()
{
	//ToggleAim(false); -- problematic place at the moment, easy fix
	UKismetSystemLibrary::PrintString(this, "CancelUse");
}

void AWeaponBase::ToggleAim(bool isAiming)
{
	auto* OwnerChar = Cast<ACharacter>(GetOwner());
	float WalkSpeedDelta = 200;

	if (isAiming)
	{
		OwnerChar->GetCharacterMovement()->MaxWalkSpeed -= WalkSpeedDelta;
	}
	else
	{
		OwnerChar->GetCharacterMovement()->MaxWalkSpeed += WalkSpeedDelta;
	}
}


// TODO: Choose projectile or hitscan depending on the weapon's type
void AWeaponBase::Shoot()
{
	CalculateLineTrace();
}

// TODO: Projectile possibility
void AWeaponBase::LaunchProjectile()
{
	//AddRecoil();
}

void AWeaponBase::CalculateLineTrace()
{
	FVector TraceStart = WeaponMesh->GetSocketLocation(MuzzleSocketName);
	FVector TraceEnd = WeaponMesh->GetSocketLocation(MuzzleSocketName) + WeaponMesh->GetForwardVector()*10000;
	const TArray<AActor*> ActorsToIgnore = {this, GetOwner()};
	FHitResult HitResult;

	if (!UKismetSystemLibrary::LineTraceSingle(this, TraceStart, TraceEnd, TraceChannel, false, ActorsToIgnore, TraceDebug, HitResult, true))
	{
		return;
	}

	if (!HitResult.bBlockingHit)
	{
		return;
	}

	if (!IsValid(HitResult.GetActor())) 
	{
		return;
	}

	// TODO: ADD DAMAGE FROM STATS STRUCT
	UGameplayStatics::ApplyDamage(HitResult.GetActor(), 25, GetInstigatorController(), this, UDamageType::StaticClass());
	//AddRecoil();
}

