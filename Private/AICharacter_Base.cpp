// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter_Base.h"
#include "../Public/AICharacter_Base.h"

#include "VisionComponent.h"
#include "GuardPoint_Base.h"
#include "PlayerBase.h"
#include "EnemyAIController_Base.h"
#include "StateMachineComponent.h"

#include "Engine.h"
#include "TimerManager.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"

#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"

#include "State_Idle.h"

AAICharacter_Base::AAICharacter_Base()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisionComponent = CreateDefaultSubobject<UVisionComponent>(TEXT("VisionComponent"));
	VisionComponent->SetupAttachment(RootComponent);

	StateMachineComponent = CreateDefaultSubobject<UStateMachineComponent>(TEXT("StateMachineComponent"));

	CirclingOffset = AvoidanceOffset;
}

void AAICharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAiOn == false)  { return;  }

	EnemySightArray = VisionComponent->GetPawnsInSight(EnemyWorldArray, VisionAngle, VisionDistance, bShowRays, bShowData);

	if (NearestTarget == nullptr)
	{
		NearestTarget = Cast<APlayerBase>(VisionComponent->GetNearestInSight(EnemySightArray, APlayerBase::StaticClass()));
	}

	if (NearestTarget && CheckEnemyInLight() == false)
	{
		TargetLastKnownPos = NearestTarget->GetActorLocation();
		Destination = GetEdgeOfRadius(CirclingOffset, TargetLastKnownPos);
	}

	if (StateMachineComponent)
	{
		StateMachineComponent->ManualUpdate();

		if (bTargetMarked == false)
		{
			TicketTarget();
		}

		if (bHasTicket)
		{
			CirclingOffset = AttackingOffset;
		}
		else
		{
			CirclingOffset = AvoidanceOffset;
		}
	}
}

void AAICharacter_Base::BeginPlay()
{
	Super::BeginPlay();

	UState_Idle* newState = NewObject<UState_Idle>();
	StateMachineComponent->SetInitialState(newState);

	StateMachineComponent->SetMyCharacter();

	if (GetController())
	{
		CurrentController = Cast<AAIController>(GetController());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Controller is invalid"));
	}

	// Get all enemies in the level as an array of actors
	SetActorsToUse();

	SetNearestGuardPoint();

	SetRandomPoint();
}

// Called to bind functionality to input
void AAICharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAICharacter_Base::SetActorsToUse()
{
	EnemyWorldArray = GetActorsToUse(EnemyClass);
}

TArray<AActor*> AAICharacter_Base::GetActorsToUse(TSubclassOf<AActor> ClassToFind)
{
	TArray<AActor*> ActorsToUse;
	UGameplayStatics::GetAllActorsOfClass(this, ClassToFind, ActorsToUse);
	return ActorsToUse;
}

// Finds and sets the nearest guard point
void AAICharacter_Base::SetNearestGuardPoint()
{
	FVector currentLocation = GetActorLocation();
	TArray<AActor*> PointsNearMe;
	AActor* NearestPoint = nullptr;
	float NearestDistance = INT_MAX;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGuardPoint_Base::StaticClass(), PointsNearMe);

	for (int32 i = 0; i < PointsNearMe.Num(); i++)
	{
		dist = FVector::Dist2D(PointsNearMe[i]->GetActorLocation(), currentLocation);

		if (dist < NearestDistance && dist < GuardPointCheckDist)
		{
			NearestPoint = PointsNearMe[i];
			NearestDistance = dist;
		}
		else if (dist > GuardPointCheckDist)
		{
			NearestPoint = this;
		}
	}
	NearestGuardPoint = NearestPoint;
}

AActor * AAICharacter_Base::GetNearestGuardPoint()
{
	return NearestGuardPoint;
}

// Finds a random navigable point and sets the target destination as that
FVector AAICharacter_Base::SetRandomPoint()
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
	{
		return FVector::ZeroVector;
	}

	FVector origin = FVector::ZeroVector;
	if (NearestGuardPoint)
	{
		origin = NearestGuardPoint->GetActorLocation();
	}
	else
	{
		origin = GetActorLocation();
	}

	FNavLocation location;
	FSharedConstNavQueryFilter emptyFilter;
	if (GetWorld())
	{
		NavSys->GetRandomReachablePointInRadius(origin, GuardDistance, location, NULL, emptyFilter);
	}

	//DrawDebugSphere(GetWorld(), location.Location, 50.0f, 10.0f, FColor::Cyan, false, 20.0f, 100, 5.0f);

	Destination = location.Location;

	return location.Location;
}

float AAICharacter_Base::GetDistToDestination()
{
	return  FVector::Dist2D(GetActorLocation(), Destination);
}

void AAICharacter_Base::SetIsChasing(bool inIsChasing)
{
	if (inIsChasing)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxChaseSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWanderSpeed;
	}
}

bool AAICharacter_Base::CheckEnemyInLight()
{
	if (NearestTarget->IsA(APlayerBase::StaticClass()))
	{
		APlayerBase* Target = Cast<APlayerBase>(NearestTarget);

		if (Target->GetIsInLight())
		{
			return true;
		}
	}
	return false;
}

bool AAICharacter_Base::CheckInAttackRange()
{
	if (true)
	{
		return true;
	}
	return false;
}

bool AAICharacter_Base::IsAtDestination()
{

	if (GetDistToDestination() <= AcceptanceRange)
	{
		return true;
	}

	return false;
}

bool AAICharacter_Base::TicketTarget()
{
	if (NearestTarget != nullptr)
	{
		bTargetMarked = NearestTarget->TryAddTicket(bIsRanged, this);
	}
	return bTargetMarked;
}

void AAICharacter_Base::RemoveTicket()
{
	if (NearestTarget)
	{
		bHasTicket = NearestTarget->RemoveTicket(bIsRanged, bIsDead, this);
	}
	else
	{
		bHasTicket = false;
	}
}

void AAICharacter_Base::SetTicket(bool bGetsTicket)
{
	if (this == nullptr) { return; }
	if (bGetsTicket)
	{
		bHasTicket = true;
		GetWorld()->GetTimerManager().SetTimer(EmptyTimerHandle, this, &AAICharacter_Base::TicketTimer, TicketTime, false);
	}
	else
	{
		bHasTicket = false;
	}
}

void AAICharacter_Base::TicketTimer()
{
	if (NearestTarget != nullptr)
	{
		NearestTarget->RemoveTicket(bIsRanged, bIsDead, this);
	}
}

void AAICharacter_Base::Die()
{
	RemoveTicket();
}

// Gets the point at a radius from their target directly backwards
FVector AAICharacter_Base::GetEdgeOfRadius(float inRange, FVector inPosition)
{
	FVector direction = UKismetMathLibrary::GetDirectionUnitVector(inPosition, GetActorLocation());

	FVector point = (direction * inRange) + inPosition;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
	{
		return FVector::ZeroVector;
	}

	FNavLocation location;
	FSharedConstNavQueryFilter emptyFilter;
	if (GetWorld())
	{
		NavSys->GetRandomReachablePointInRadius(point, 500, location, NULL, emptyFilter);
	}
	
	//return location.Location;
	return point;
}

void AAICharacter_Base::Attack_Implementation()
{
}

void AAICharacter_Base::Special_Implementation()
{
}

bool AAICharacter_Base::HasTicket() { return bHasTicket; }