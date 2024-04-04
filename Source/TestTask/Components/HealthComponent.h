// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeath, AController*, Instigator, AActor*, DamageCauser);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTTASK_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastDeath(AController* Instigator, AActor* DamageCauser);
	void Multicast_BroadcastDeath_Implementation(AController* Instigator, AActor* DamageCauser) { OnDeath.Broadcast(Instigator, DamageCauser); }

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastHealthChange(float NewValue);
	void Multicast_BroadcastHealthChange_Implementation(float NewValue) { OnHealthChanged.Broadcast(NewValue); }

	void Damage(float IncomingDamage, AController* Instigator, AActor* DamageCauser);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void Server_Damage(float IncomingDamage, AController* Instigator, AActor* DamageCauser);
	void Server_Damage_Implementation(float IncomingDamage, AController* Instigator, AActor* DamageCauser) { Damage(IncomingDamage, Instigator, DamageCauser); }


	void AddHealth(float NewHealth);
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health")
	void Server_AddHealth(float NewHealth);
	void Server_AddHealth_Implementation(float NewHealth) { AddHealth(NewHealth); }
	

	UFUNCTION(BlueprintPure, Category = "Health")
	const float GetHealth();

	UFUNCTION(BlueprintPure, Category = "Health")
	const float GetMaxHealth();

	UFUNCTION(BlueprintPure, Category = "Health")
	const bool GetIsDead();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	//Replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:
	UPROPERTY(Replicated)
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Health", meta = (ClampMin = 1.0f), Replicated)
	float MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Health", Replicated)
	bool bIsInvincible;

	UPROPERTY(VisibleAnywhere, Category = "Health", Replicated)
	bool bIsDead;
};
