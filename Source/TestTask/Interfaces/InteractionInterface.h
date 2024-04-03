// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, meta = (CannotImplementInterfaceInBlueprint = true))
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TESTTASK_API IInteractionInterface
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void Interact(APawn* Interactor);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual bool StartPrimaryUse();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual bool StartSecondaryUse();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void StopUse();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void CancelUse();
};
