// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SolaceCharacter.h"
#include "AICharacter_Base.generated.h"

class AAIController;
class APlayerBase;
class AEnemySpawner_Base;

/**
 * 
 */
UCLASS()
class SOLACE_API AAICharacter_Base : public ASolaceCharacter
{
	GENERATED_BODY()

public:
	AAICharacter_Base();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

protected:

	virtual void BeginPlay() override;

	// Debugging variables
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debugging)
		bool bShowRays;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debugging)
		bool bShowData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomAI)
		bool bAiOn = true;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Perception)
		ASolaceCharacter* NearestTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Perception)
		FVector TargetLastKnownPos;

	/*How far away they should stay from their target*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AIMovement)
		float CirclingOffset = 100.0f;

	/*How far away they should stay when attacking*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMovement)
		float AttackingOffset = 100.0f;

	/*How far away they should stay when they aren't attacking*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMovement)
		float AvoidanceOffset = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomAI)
		float GuardDistance = 10000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CustomAI)
		bool bIsInRange;

	/*Used for various distance checks, global so I can see it in editor*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CustomAI)
		float dist;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CustomAI)
		AActor* NearestGuardPoint;

	/*If false, patrol*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomAI)
		bool bShouldGuard;

	UPROPERTY(BlueprintReadWrite, Category = StateMachine)
		bool bIsInState;

	/*Attack range*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		bool bCanAttack = true;

	UPROPERTY(BlueprintReadWrite, Category = Attack)
		bool bCanSpecial = true;

	UPROPERTY(BlueprintReadWrite, Category = Attack)
		bool bIsOnFire;

	/*How close it has to be to accept that it reached its target*/
	UPROPERTY(EditAnywhere, Category = AIMovement)
		float AcceptanceRange = 100.0f;

	/*Where it wants to go*/
	UPROPERTY(VisibleAnywhere)
		FVector Destination;

	UPROPERTY(BlueprintReadWrite)
		bool Hurting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bArePatrolling = true;

public:

	/*How long the ai is allowed to use a ticket*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ticketing")
		float TicketTime = 10.0f;

	/*How fast it should go while chasing its target*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMovement)
		float MaxChaseSpeed = 600.0f;

	/*Time betweeen attacks*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float AttackCooldown;

	/*Damage*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float Damage = 1;

	UPROPERTY(VisibleAnywhere, Category = "Ticketing")
		bool bHasTicket = false;

protected:

	FTimerHandle EmptyTimerHandle;

	UPROPERTY(BlueprintReadOnly)
		class UStateMachineComponent* StateMachineComponent;

	UPROPERTY(BlueprintReadOnly, Category = Perception)
		class UVisionComponent* VisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Perception)
		TArray<AActor*> EnemyWorldArray;

	UPROPERTY(BlueprintReadWrite, Category = Perception)
		TArray<AActor*> EnemySightArray;

	/* Angle from forward to desired angle, or half of desired view cone*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
		float VisionAngle;

	/*Distance to detect*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
		float VisionDistance;

	/*Does nothing*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
		float HearingDistance;

	/*How long from the time the special ends to the time it can start again*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float SpecialCooldown;

	/*Time special lasts*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		float SpecialTime;

	/*Which class should be its enemy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
		TSubclassOf<AActor> EnemyClass;

	/*How fast it should go while patrolling*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIMovement)
		float MaxWanderSpeed = 300.0f;

	/*Distance the ai looks for guard points around them*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CustomAI)
		float GuardPointCheckDist = 10000;

	UPROPERTY(BlueprintReadWrite, Category = CustomAI)
		AAIController* CurrentController;

	/*Spawner that the AI spawned from*/
	UPROPERTY(BlueprintReadWrite, Category = CustomAI)
		AEnemySpawner_Base* SpawnedFrom;

	// Animation variables
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CustomAI)
		bool bIsAttacking;

private:

	UPROPERTY(VisibleAnywhere, Category = "Ticketing")
		bool bTargetMarked = false;

public:

	UFUNCTION(BlueprintCallable, Category = CustomAI)
		void SetActorsToUse();

	UFUNCTION(BlueprintCallable, Category = CustomAI)
		TArray<AActor*> GetActorsToUse(TSubclassOf<AActor> ClassToFind);

	UFUNCTION(BlueprintCallable, Category = CustomAI)
		void SetNearestGuardPoint();

	UFUNCTION(BlueprintCallable, Category = CustomAI)
		AActor* GetNearestGuardPoint();

	UFUNCTION(BlueprintCallable, Category = CustomAI)
		FVector SetRandomPoint();

	UFUNCTION(BlueprintCallable, Category = CustomAI)
		float GetDistToDestination();

	UFUNCTION(BlueprintCallable, Category = CustomAI)
		TArray<AActor*> GetWorldActors() { return EnemyWorldArray; }

	UFUNCTION(BlueprintNativeEvent)
		void Attack();

	UFUNCTION(BlueprintNativeEvent)
		void Special();

	void SetSpawnedFrom(AEnemySpawner_Base* inSpawnedFrom) { SpawnedFrom = inSpawnedFrom; }

	void SetIsChasing(bool inIsChasing);

	// State machine functions
public:

	UFUNCTION()
		bool CheckEnemyInLight();

	UFUNCTION()
		bool CheckInAttackRange();

	UFUNCTION()
		bool IsAtDestination();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Ticketing")
		bool bIsRanged;

public:

	/*Removes itself from the ticketing system of the target*/
	void RemoveTicket();

	/* If the target isn't fully ticketed take another turn */
	UFUNCTION()
		void SetTicket(bool bGetsTicket);

	UFUNCTION()
		bool HasTicket();

protected:

	UFUNCTION()
		FVector GetEdgeOfRadius(float inRange, FVector inPosition);

	/*Tries to add itself to the ticketing system of nearest target*/
	bool TicketTarget();

	virtual void Die() override;

	private:

		UFUNCTION()
			void TicketTimer();
};
