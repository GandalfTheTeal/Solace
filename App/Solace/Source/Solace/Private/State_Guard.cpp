// Fill out your copyright notice in the Description page of Project Settings.

#include "State_Guard.h"
#include "State_Chase.h"
#include "State_Idle.h"

void UState_Guard::Enter(UStateMachineComponent* inSM)
{
	Super::Enter(inSM);
	if (MySM->GetMyCharacterController()->NearestGuardPoint)
	{
		MySM->GetMyCharacterController()->Move(MySM->GetMyCharacterController()->NearestGuardPoint->GetActorLocation(), MySM->GetMyCharacter()->AcceptanceRange);
	}
}

void UState_Guard::Update()
{
	if (MySM->GetMyCharacterController()->GetIsPlayerInRange() == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Chase"));
		UState_Chase* newState = NewObject<UState_Chase>();
		MySM->SetNextState(newState);
	}
	else if(MySM->GetMyCharacter()->IsAtDestination() == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Idle"));
		UState_Idle* newState = NewObject<UState_Idle>();
		MySM->SetNextState(newState);
	}
	UE_LOG(LogTemp, Warning, TEXT("Nothing"));
}

void UState_Guard::Exit()
{
}

/*
EnemyInSight
TargetDist
*/