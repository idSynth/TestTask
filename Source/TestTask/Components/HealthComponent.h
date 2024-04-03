// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Damage(float IncomingDamage, AController* Instigator, AActor* DamageCauser, bool &bIsDead);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float NewHealth);

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

private:
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Health", meta = (ClampMin = 1.0f))
	float MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	bool bIsInvincible;

	bool bIsDead;
};
