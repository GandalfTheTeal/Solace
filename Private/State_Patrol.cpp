// Fill out your copyright notice in the Description page of Project Settings.

#include "State_Patrol.h"
#include "State_Chase.h"

void UState_Patrol::Enter(UStateMachineComponent* inSM)
{
	Super::Enter(inSM);

	bShouldUpdate = true;

	MySM->GetMyCharacter()->SetRandomPoint();
	MySM->GetMyCharacterController()->MoveToLocation(MySM->GetMyCharacter()->Destination, MySM->GetMyCharacter()->AcceptanceRange, false, true, false, true, 0, true);
	
	MySM->GetMyCharacter()->SetIsChasing(false);
}

void UState_Patrol::Update()
{
	if (bShouldUpdate == false || MySM->GetMyCharacterController() == nullptr || MySM->GetMyCharacter() == nullptr) { return; }

	if (MySM->GetMyCharacter()->NearestTarget)
	{
		UState_Chase* newState = NewObject<UState_Chase>();
		MySM->SetNextState(newState);
	}

	if (MySM->GetMyCharacter()->IsAtDestination())
	{
		MySM->GetMyCharacter()->SetRandomPoint();
		MySM->GetMyCharacterController()->MoveToLocation(MySM->GetMyCharacter()->Destination, MySM->GetMyCharacter()->AcceptanceRange, false, true, false, true, 0, true);
	}
}

void UState_Patrol::Exit()
{
}