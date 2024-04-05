// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTask/Items/WeaponBase.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Character.h"
#include "TestTask/Interfaces/WeaponAnimInterface.h"


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
	switch (Stats.FireMode)
	{
		case Single:
		{
			Server_Shoot();
			break;
		}
		case Auto:
		{
			GetWorldTimerManager().SetTimer(FiringTimer, this, &AWeaponBase::ShootWrapper, 1.0f / Stats.FireRate, true);
			break;
		}
	}
	UKismetSystemLibrary::PrintString(this, "StartPrimaryUse");
	return true;
}

void AWeaponBase::ShootWrapper()
{
	if (HasAuthority())
	{
		Shoot();
	}
	else
	{
		Server_Shoot();
	}
}

// TODO: Choose projectile or hitscan depending on the weapon's type
void AWeaponBase::Shoot()
{
	if (Stats.AmmoInMagazine <= 0)
	{
		GetWorldTimerManager().ClearTimer(FiringTimer);
		StopPrimaryUse();
		return;
	}

	//switch (WeaponData->WeaponType)      -      WeaponType doesn't exist at the moment
	//{
	//	case Hitscan:
	//	{
			Server_CalculateLineTrace();
			Stats.AmmoInMagazine--;
			Client_BroadcastAmmoChanged(Stats);
	//	}
	//	case Projectile:
	//	{

	//	}
	//}

	Multicast_PlayShootingMontage();
}

void AWeaponBase::PlayShootingMontage()
{
	UAnimInstance* OwnerAnimInstance = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();
	if (!IsValid(OwnerAnimInstance))
	{
		return;
	}

	Cast<IWeaponAnimInterface>(OwnerAnimInstance)->Execute_PlayShootingMontage(OwnerAnimInstance);

	if (WeaponData->WeaponShootingMontage)
	{
		WeaponMesh->GetAnimInstance()->Montage_Play(WeaponData->WeaponShootingMontage);
	}
}

bool AWeaponBase::StartSecondaryUse()
{
	Server_ToggleAim(true);
	UKismetSystemLibrary::PrintString(this, "StartSecondaryUse");
	return true;
}

void AWeaponBase::StopPrimaryUse()
{
	GetWorldTimerManager().ClearTimer(FiringTimer);
	UKismetSystemLibrary::PrintString(this, "StopPrimaryUse");
}


void AWeaponBase::StopSecondaryUse()
{
	Server_ToggleAim(false);
	UKismetSystemLibrary::PrintString(this, "StopSecondaryUse");
}


bool AWeaponBase::StartUtilityUse()
{
	if (Stats.AmmoInMagazine == Stats.MaxAmmoInMagazine)
	{
		return false;
	}

	Server_ReloadStart();
	Server_ReloadEnd();
	return true;
}

void AWeaponBase::CancelUse()
{
	StopPrimaryUse();
	StopSecondaryUse();
	UKismetSystemLibrary::PrintString(this, "CancelUse");
}

void AWeaponBase::ToggleAim(bool bIsAiming)
{
	UAnimInstance* OwnerAnimInstance = Cast<ACharacter>(GetOwner())->GetMesh()->GetAnimInstance();
	if (!IsValid(OwnerAnimInstance))
	{
		return;
	}

	Cast<IWeaponAnimInterface>(OwnerAnimInstance)->Execute_SetAiming(OwnerAnimInstance, bIsAiming);

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
	UGameplayStatics::ApplyDamage(HitResult.GetActor(), Stats.Damage, GetInstigatorController(), this, UDamageType::StaticClass());
	//AddRecoil();
}

// TODO: Choose projectile or hitscan depending on the weapon's type
void AWeaponBase::ReloadStart()
{

	if (Stats.AmmoInMagazine == Stats.MaxAmmoInMagazine)
	{
		return;
	}

	Stats.SpareAmmo += Stats.AmmoInMagazine;
	Stats.AmmoInMagazine = 0;
	Client_BroadcastAmmoChanged(Stats);
}


void AWeaponBase::ReloadEnd()
{
	Stats.AmmoInMagazine = FMath::Min(Stats.MaxAmmoInMagazine, Stats.SpareAmmo);
	Stats.SpareAmmo -= FMath::Min(Stats.MaxAmmoInMagazine, Stats.SpareAmmo);
	Client_BroadcastAmmoChanged(Stats);
}


void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponBase, Stats);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (!WeaponData)
	{
		Destroy();
	}

	if (WeaponData)
	{
		WeaponMesh->SetSkeletalMesh(WeaponData->Mesh);
		Slot = WeaponData->Slot;
		Stats = WeaponData->InitStats;
	}
}
