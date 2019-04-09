// Fill out your copyright notice in the Description page of Project Settings.

#include "State_Chase.h"

void UState_Chase::Enter(UStateMachineComponent* inSM)
{
	Super::Enter(inSM);

	LookAt();
	
	MySM->GetMyCharacter()->SetIsChasing(true);
}

void UState_Chase::Update()
{
	if (bShouldUpdate == false || MySM->GetMyCharacterController() == nullptr || MySM->GetMyCharacter() == nullptr) { return; }

	if (MySM->GetMyCharacter()->NearestTarget)
	{
		MySM->GetMyCharacterController()->MoveToLocation(MySM->GetMyCharacter()->Destination, MySM->GetMyCharacter()->AcceptanceRange, false, true, false, true, 0, true);
	}

}

void UState_Chase::Exit()
{
}

void UState_Chase::LookAt()
{
	bShouldUpdate = true;
}

/*
distance to enemy
has reached the last point to chase
enemy in light
*/