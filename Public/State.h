// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "StateMachineComponent.h"
#include "EnemyAIController_Base.h"
#include "AICharacter_Base.h"

#include "State.generated.h"

class UStateMachineComponent;

/**
 * 
 */

UCLASS()
class SOLACE_API UState : public UObject
{
	GENERATED_BODY()

public:

	UState(){}
	
protected:

	UPROPERTY()
	UStateMachineComponent* MySM;

protected:

	UPROPERTY()
		bool bShouldUpdate;

public:

	UFUNCTION()
		virtual void Enter(UStateMachineComponent* inSM);

	UFUNCTION()
		virtual void Update();

	UFUNCTION()
		virtual void Exit();
	
	
};
