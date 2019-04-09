// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController_Base.generated.h"

/**
 * 
 */

class UStateMachineComponent;

/**
*
*/

UCLASS()
class SOLACE_API AEnemyAIController_Base : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController_Base();

private:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(class APawn* PossessedPawn);
	
public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<AActor*> ActorsToUseFromWorld; 

	UPROPERTY(BlueprintReadWrite, Category = CustomAI)
		AActor* NearestGuardPoint;

	UPROPERTY(BlueprintReadWrite, Category = CustomAI)
		AActor* NearestPatrolPoint;

	UPROPERTY(BlueprintReadWrite, Category = CustomAI)
		bool bShouldSpecial = true;

protected:

	UPROPERTY(BlueprintReadOnly)
		class AAICharacter_Base* MyPawn;

	UStateMachineComponent* EnemyStateMachine;

	float DistToGuardPoint;

	float EnemyDistToGuardPoint;

public:

	UFUNCTION(BlueprintCallable, Category = CustomAI)
		TArray < AActor*> GetPawnsInEarshot(TArray<AActor*> ActorsToUse, float HearingDistance);		// Returns an array of all pawns that are making a sound in a radius

	UFUNCTION(BlueprintCallable, Category = CustomAI)
		bool GetIsRight(AActor* Target);		// Returns true if the target is on the right of the character

	void Move(FVector Location, float AcceptanceRadius);

	AAICharacter_Base* GetMyPawn() { return MyPawn; }
	

	// Statemachine stuff
protected:

	/*void ChooseState();

	void RunBadStateMachine(int32 StateNum);

	UFUNCTION(BlueprintNativeEvent)
		void Idle();

	UFUNCTION(BlueprintNativeEvent)
		void Chase();

	UFUNCTION(BlueprintNativeEvent)
		void Patrol();

	UFUNCTION(BlueprintNativeEvent)
		void Guard();*/

	UFUNCTION(BlueprintCallable)
		bool CheckShouldChase();

	UFUNCTION(BlueprintCallable)
		void CheckShouldAttack();

	UFUNCTION(BlueprintCallable)
		void CheckShouldSpecial();

	UFUNCTION(BlueprintCallable)
		void CheckIsInRange();

public:

	bool GetIsPlayerInSight();

	bool GetIsPlayerInRange();

	bool GetIsInChaseRange();

	bool CheckIsAtLastPos();

private:

	FVector GetEdgeOfRadius(float inRange, FVector inPosition);
};
