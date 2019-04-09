// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "State_Guard.generated.h"

/**
 * 
 */
UCLASS()
class SOLACE_API UState_Guard : public UState
{
	GENERATED_BODY()
	
public:

	virtual void Enter(UStateMachineComponent* inSM) override;

	virtual void Update() override;

	virtual void Exit() override;
	
};
