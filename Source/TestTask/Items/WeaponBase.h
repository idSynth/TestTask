// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestTask/Items/PickupableActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TestTask/Items/Data/WeaponData.h"
#include "Net/UnrealNetwork.h"
#include "WeaponBase.generated.h"

class ACharacter;
class UCharacterMovementComponent;
class IWeaponAnimInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, FWeaponStats, NewAmmo);

/**
 * 
 */
UCLASS(meta = (PrioritizeCategories = "Stats"))
class TESTTASK_API AWeaponBase : public APickupableActor
{
	GENERATED_BODY()
	
public:
	
	AWeaponBase();

	UPROPERTY(VisibleAnywhere, Category = "Visuals")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = "Stats")
	UWeaponData* WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", Replicated)
	FWeaponStats Stats;

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

	bool StartUtilityUse() override;

	void StopPrimaryUse() override;

	void StopSecondaryUse() override;

	void CancelUse() override;

	// Interface implementation ends

	void ToggleAim(bool isAiming);
	UFUNCTION(Server, Reliable)
	void Server_ToggleAim(bool isAiming);
	void Server_ToggleAim_Implementation(bool isAiming) { Multicast_ToggleAim(isAiming); }
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ToggleAim(bool isAiming);
	void Multicast_ToggleAim_Implementation(bool isAiming) { ToggleAim(isAiming); }

	void PlayShootingMontage();
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayShootingMontage();
	void Multicast_PlayShootingMontage_Implementation() { PlayShootingMontage(); }


	void Shoot();
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Weapon")
	void Server_Shoot();
	void Server_Shoot_Implementation() { Shoot(); }
	void ShootWrapper(); //Wrapper function to call a server function from timer

	void LaunchProjectile();
	UFUNCTION(Server, Reliable)
	void Server_LaunchProjectile();
	void Server_LaunchProjectile_Implementation() { LaunchProjectile(); }

	void CalculateLineTrace();
	UFUNCTION(Server, Reliable)
	void Server_CalculateLineTrace();
	void Server_CalculateLineTrace_Implementation() { CalculateLineTrace(); }
	
	void ReloadStart();
	UFUNCTION(Server, Reliable)
	void Server_ReloadStart();
	void Server_ReloadStart_Implementation() { ReloadStart(); }

	void ReloadEnd();
	UFUNCTION(Server, Reliable)
	void Server_ReloadEnd();
	void Server_ReloadEnd_Implementation() { ReloadEnd(); }

	//Replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
	FOnAmmoChanged OnAmmoChanged;
	UFUNCTION(Client, Unreliable)
	void Client_BroadcastAmmoChanged(FWeaponStats NewStats);
	void Client_BroadcastAmmoChanged_Implementation(FWeaponStats NewStats) { OnAmmoChanged.Broadcast(NewStats); }


protected:

	virtual void BeginPlay();



private:

	// TODO: Recoil
	void AddRecoil();

	FTimerHandle FiringTimer;

};

