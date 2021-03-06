// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "State_Idle.generated.h"

/**
 * 
 */
UCLASS()
class SOLACE_API UState_Idle : public UState
{
	GENERATED_BODY()
	
public:

	virtual void Enter(UStateMachineComponent* inSM) override;

	virtual void Update() override;

	virtual void Exit() override;
};
