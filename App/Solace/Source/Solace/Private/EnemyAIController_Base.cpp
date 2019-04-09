
#include "EnemyAIController_Base.h"

#include "Engine/World.h"

#include "PlayerBase.h"
#include "AICharacter_Base.h"
#include "StateMachineComponent.h"

#include <math.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/NavigationSystem/Public/NavFilters/RecastFilter_UseDefaultArea.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

// Fill out your copyright notice in the Description page of Project Settings.

AEnemyAIController_Base::AEnemyAIController_Base()
{
	
}

void AEnemyAIController_Base::BeginPlay()
{
	Super::BeginPlay();

	if (GetPawn() && GetPawn()->IsA<AAICharacter_Base>())
	{
		MyPawn = Cast<AAICharacter_Base>(GetPawn());
	}
}

void AEnemyAIController_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MyPawn) 
	{ 
		if (GetPawn() && GetPawn()->IsA<AAICharacter_Base>())
		{
			MyPawn = Cast<AAICharacter_Base>(GetPawn());
		}
		else
		{
			return; 
		}
	}

	if (MyPawn->GetIsDead()) { 
		return; 
	}

	if (MyPawn->NearestTarget)
	{
		FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(MyPawn->GetActorLocation(), MyPawn->NearestTarget->GetActorLocation());
		ControlRotation = LookAt;
		SetFocus(MyPawn->NearestTarget);
		AActor* myFocus = GetFocusActor();
	}
	else
	{
		ClearFocus(EAIFocusPriority::Gameplay);
	}

	// Debugging for turning off ai
	if (!MyPawn->bAiOn) { return; }

	CheckIsInRange();

	CheckShouldAttack();
	
	if (bShouldSpecial)
	{
		CheckShouldSpecial();
	}
	
	//ChooseState();

}

void AEnemyAIController_Base::OnPossess(APawn * PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	if (PossessedPawn && PossessedPawn->IsA<AAICharacter_Base>())
	{
		MyPawn = Cast<AAICharacter_Base>(PossessedPawn);

		//MyPawn->SetRandomPoint();
	}
}

TArray<AActor*> AEnemyAIController_Base::GetPawnsInEarshot(TArray<AActor*> ActorsToUse, float HearingDistance)
{
	TArray<AActor*> emptyArray;
	if (ActorsToUse.Num() <= 0 || GetPawn() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetPawnsInEarshot input or pawn invalid"));
		return emptyArray;
	}

	TArray<AActor*> actorsInEarshot;
	APawn* controlledPawn = GetPawn();
	FVector currentLocation = controlledPawn->GetActorLocation();

	for (int i = 0; i < ActorsToUse.Num(); i++)
	{
		if (ActorsToUse[i] && ActorsToUse[i] != controlledPawn && !ActorsToUse[i]->IsPendingKill())
		{
			float distFromMe = FVector::Dist2D(ActorsToUse[i]->GetActorLocation(), currentLocation);
			//bool bisMakingNoise = Cast<ASolaceCharacter>(ActorsToUse[i])->GetIsMakingNoise();

			if (distFromMe <= HearingDistance /*&& bisMakingNoise*/)
			{
				FHitResult ActorHitResults;

				FVector traceFrom = GetPawn()->GetActorLocation();
				traceFrom.Z += 150;

				GetWorld()->LineTraceSingleByChannel(ActorHitResults, traceFrom, currentLocation, ECollisionChannel::ECC_WorldStatic);
				if (ActorHitResults.Actor == ActorsToUse[i])
				{
					actorsInEarshot.AddUnique(ActorsToUse[i]);
				}
			}
		}
	}
	return actorsInEarshot;
}

bool AEnemyAIController_Base::GetIsRight(AActor * Target)
{
	if (Target == nullptr || GetPawn() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetIsRight was fed a null target or pawn was null"));
		return false;
	}

	APawn* controlledPawn = GetPawn();
	FVector currentLocation = controlledPawn->GetActorLocation();

	FVector pointRight = controlledPawn->GetActorRightVector();
	pointRight += currentLocation;

	FVector pointLeft = controlledPawn->GetActorRightVector() * -1;
	pointLeft += currentLocation;

	FVector targetLocation = Target->GetActorLocation();

	float leftDist = FVector::Dist2D(pointLeft, targetLocation);
	float rightDist = FVector::Dist2D(pointRight, targetLocation);

	if (leftDist > rightDist)
	{
		return true;
	}

	return false;
}

void AEnemyAIController_Base::Move(FVector Location, float AcceptanceRadius)
{
	MoveToLocation(Location, AcceptanceRadius, false, true, false, false, URecastFilter_UseDefaultArea::StaticClass(), true);
}

bool AEnemyAIController_Base::GetIsPlayerInSight()
{
	if (MyPawn->NearestTarget != nullptr)
	{
		return true;
	}
	return false;
}

bool AEnemyAIController_Base::CheckShouldChase()
{
	return true;
}

void AEnemyAIController_Base::CheckShouldAttack()
{
	if (MyPawn->NearestTarget == nullptr) { return; }
	if (MyPawn->bCanAttack && MyPawn->bHasTicket && MyPawn->GetIsDead() == false && MyPawn->NearestTarget->GetIsDead() == false)
	{
		float dist = FVector::Dist(MyPawn->NearestTarget->GetActorLocation(), MyPawn->GetActorLocation());
		if (dist < MyPawn->AttackRange)
		{
			MyPawn->Attack();
		}
	}
}

void AEnemyAIController_Base::CheckShouldSpecial()
{
	if (MyPawn->NearestTarget && MyPawn->bCanSpecial)
	{
		MyPawn->Special();
	}
}

void AEnemyAIController_Base::CheckIsInRange()
{
	if (!MyPawn) {
		return;
	}

	if (MyPawn->NearestTarget)
	{
		float enemyDist = FVector::Dist2D(MyPawn->GetActorLocation(), MyPawn->NearestTarget->GetActorLocation());
		if (enemyDist <= MyPawn->AttackRange - MyPawn->AttackRange)
		{
			MyPawn->bIsInRange = true;
		}
		else
		{
			MyPawn->bIsInRange = false;
		}
	}
	else
	{
		MyPawn->bIsInRange = false;
	}

//Eru Ilúvatar
//Maedhros
//Thingol
//Beleg
//Geoffrey
}

bool AEnemyAIController_Base::GetIsPlayerInRange()
{
	if (MyPawn->NearestTarget == nullptr)
	{
		return false;
	}

	float dist = FVector::Dist(MyPawn->NearestTarget->GetActorLocation(), MyPawn->GetActorLocation());

	if (dist < MyPawn->GuardDistance)
	{
		return true;
	}
	return false;
}

bool AEnemyAIController_Base::GetIsInChaseRange()
{
	return false;
}

bool AEnemyAIController_Base::CheckIsAtLastPos()
{
	return false;
}

FVector AEnemyAIController_Base::GetEdgeOfRadius(float inRange, FVector inPosition)
{
	FVector direction = UKismetMathLibrary::GetDirectionUnitVector(inPosition, MyPawn->GetActorLocation());

	FVector point = (direction * inRange) + inPosition;
	return point;
}
