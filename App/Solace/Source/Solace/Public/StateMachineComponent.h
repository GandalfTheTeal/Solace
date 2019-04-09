// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"

class UState;
class UState_Idle;
class UState_Chase;
class UState_Guard;
class UState_Patrol;

class AEnemyAIController_Base;
class AAICharacter_Base;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOLACE_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	UPROPERTY()
		AEnemyAIController_Base* MyCharacterController;

	UPROPERTY()
		AAICharacter_Base* MyCharacter;

	UPROPERTY()
		UState* CurrentState;

	UPROPERTY()
		UState* NextState;

public:

	void SetInitialState(UState* inState);

	void ManualUpdate();

	void SetNextState(UState* inState) { NextState = inState; }

	void SetMyCharacter();

	void SetMyCharacterController(AEnemyAIController_Base* inController) { MyCharacterController = inController; }

	AEnemyAIController_Base* GetMyCharacterController() { return MyCharacterController; }

	AAICharacter_Base* GetMyCharacter() { return MyCharacter; }

private:

	void EnterFirstState();
	
};
