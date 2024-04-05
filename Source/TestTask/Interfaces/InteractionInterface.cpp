// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTask/Interfaces/InteractionInterface.h"

// Add default functionality here for any IInteractionInterface functions that are not pure virtual.

void IInteractionInterface::Interact(APawn* Interactor)
{
}

bool IInteractionInterface::StartPrimaryUse()
{
	return false;
}

bool IInteractionInterface::StartSecondaryUse()
{
	return false;
}

bool IInteractionInterface::StartUtilityUse()
{
	return false;
}

void IInteractionInterface::StopPrimaryUse()
{
}

void IInteractionInterface::StopSecondaryUse()
{
}

void IInteractionInterface::CancelUse()
{
}
