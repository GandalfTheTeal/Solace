// Fill out your copyright notice in the Description page of Project Settings.

#include "State_Idle.h"
#include "State_Patrol.h"
#include "State_Guard.h"


void UState_Idle::Enter(UStateMachineComponent* inSM)
{
	Super::Enter(inSM);

	if (MySM->GetMyCharacter()->bShouldGuard)
	{
		UState_Guard* newState = NewObject<UState_Guard>();
		MySM->SetNextState(newState);
	}
	else
	{
		UState_Patrol* newState = NewObject<UState_Patrol>();
		MySM->SetNextState(newState);
	}
}

void UState_Idle::Update()
{
}

void UState_Idle::Exit()
{
}