// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTask/Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHealthComponent::Damage(float IncomingDamage, AController* Instigator, AActor* DamageCauser, bool &bDead)
{
	Health = FMath::Clamp(Health - IncomingDamage, 0, MaxHealth);;
	OnHealthChanged.Broadcast(Health);

	if (bIsInvincible)
	{
		return;
	}

	bIsDead = (Health <= 0.0f);
	bDead = bIsDead;
	if (bIsDead)
	{
		OnDeath.Broadcast(Instigator, DamageCauser);
	}
}

void UHealthComponent::AddHealth(float IncomingHealth)
{
	if (bIsDead)
	{
		return;
	}

	Health = FMath::Clamp(Health + IncomingHealth, 0, MaxHealth);
	
	OnHealthChanged.Broadcast(Health);
}

const float UHealthComponent::GetHealth()
{
	return Health;
}

const float UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}

const bool UHealthComponent::GetIsDead()
{
	return bIsDead;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	//Initialize health
	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

