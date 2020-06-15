// Fill out your copyright notice in the Description page of Project Settings.

#include "State.h"

void UState::Enter(UStateMachineComponent* inSM)
{
	MySM = inSM;
}

void UState::Update()
{
	// Implement in child classes
}

void UState::Exit()
{
	// Implement in child classes
}
