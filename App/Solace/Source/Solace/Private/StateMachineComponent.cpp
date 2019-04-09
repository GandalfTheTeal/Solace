// Fill out your copyright notice in the Description page of Project Settings.

#include "StateMachineComponent.h"

#include "State_Idle.h"
#include "State_Chase.h"
#include "State_Guard.h"
#include "State_Patrol.h"

#include "EnemyAIController_Base.h"
#include "AICharacter_Base.h"

#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine.h"


// Sets default values for this component's properties
UStateMachineComponent::UStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	FTimerHandle UnusedHandle;
	//GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UStateMachineComponent::SetMyCharacter, 0.001f);
	GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UStateMachineComponent::EnterFirstState, 0.01f);
}


// Called every frame
void UStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//Update();
}

void UStateMachineComponent::SetMyCharacter()
{
	if (GetOwner()->IsA(APawn::StaticClass()))
	{
		MyCharacter = Cast<AAICharacter_Base>(GetOwner());
		MyCharacterController = Cast<AEnemyAIController_Base>(MyCharacter->GetController());
	}
}

void UStateMachineComponent::EnterFirstState()
{
	Cast<UState>(CurrentState)->Enter(this);
}

void UStateMachineComponent::ManualUpdate()
{
	if (MyCharacterController == nullptr) { return; }

	if (CurrentState && NextState)
	{
		if (CurrentState == NextState)
		{
			CurrentState->Update();
		}
		else
		{
			CurrentState->Exit();
			CurrentState = NextState;
			CurrentState->Enter(this);
		}
	}
	else if (CurrentState)
	{
		NextState = CurrentState;
		CurrentState->Update();
	}
}

void UStateMachineComponent::SetInitialState(UState* inState)
{
	CurrentState = inState;
	NextState = inState;
}