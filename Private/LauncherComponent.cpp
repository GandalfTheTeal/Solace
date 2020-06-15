// Fill out your copyright notice in the Description page of Project Settings.

#include "LauncherComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Engine.h"

#include "ProjectileBase.h"

#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"

// Sets default values for this component's properties
ULauncherComponent::ULauncherComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

FVector ULauncherComponent::PredictLaunchVector(AActor* Target, FVector& outEndPoint, float Speed, FVector StartPosOffset, float LaunchObjectRadius)
{
	if (!Target)
	{
		return FVector::ZeroVector;
	}

	// Get position and velocity of target actor
	FVector targetLocation = Target->GetActorLocation();
	FVector targetVelocity = Target->GetVelocity();

	// Get position to throw from
	FVector LaunchPos = StartPosOffset + GetComponentLocation();

	// Save current height of target
	float Height = targetLocation.Z;

	// Get time it would take for the projectile to reach current target location
	float time = 0;

	// Get location of target in n seconds if they keep their heading and velocity
	FVector tempLocation = targetLocation;
	FVector locationToAim = targetLocation;

	for (int i = 0; i < AccuracyLoops; i++)
	{
		// Calculate approximate time it would take for the projectile to reach the new location
		time = (FVector::Dist(LaunchPos, tempLocation) / 100) / (Speed / 100);

		// Get location of the target at n seconds if they continue on their current path
		locationToAim = targetLocation + (targetVelocity * (time));
		tempLocation = locationToAim;
	}

	if (AccuracyLoops >= 0)
	{
		// Get location of the target at n seconds if they continue on their current path
		targetLocation += (targetVelocity)* (time * LeadOffset);
	}

	targetLocation.Z = Height + LaunchObjectRadius;

	// Get the target location in local space
	FVector relativeLocation = (targetLocation - LaunchPos);
	outEndPoint = relativeLocation;

	time = (relativeLocation.Size() * 0.01) / (Speed * 0.01);

	// Get required Z speed
	float zSpeed = ((980 * GravityScale) * (time * 0.5));
	//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("z = %f"), zSpeed)); }


	// Normalize the relative location of the target point
	relativeLocation.Normalize();

	FVector newVelocity = relativeLocation * Speed;
	newVelocity.Z += zSpeed;

	return newVelocity;
}


AProjectileBase* ULauncherComponent::LaunchProjectile(FVector LaunchVector, FVector LaunchPosOffset, bool bLaunchFromHere)
{
	FVector LaunchPos = FVector::ZeroVector;
	if (bLaunchFromHere)
	{
		LaunchPos = LaunchPosOffset;
	}
	else
	{
		LaunchPos = LaunchPosOffset + GetComponentLocation();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;

	FRotator newRotation = GetComponentRotation();
	AProjectileBase* ProjectileSpawned = GetWorld()->SpawnActor<AProjectileBase>(ProjectileToThrow, LaunchPos, newRotation, SpawnParams);

	if (ProjectileSpawned)
	{
		//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("x = %f, y = %f, z = %f"), LaunchPos.X, LaunchPos.Y, LaunchPos.Z)); }

		ProjectileSpawned->ProjectileComponent->Velocity = LaunchVector;
		ProjectileSpawned->ProjectileComponent->ProjectileGravityScale = GravityScale;
		return ProjectileSpawned;
	}
	
	return nullptr;
}

void ULauncherComponent::RedirectProjectile(FVector LaunchVector, AProjectileBase * inProjectile)
{
	inProjectile->ProjectileComponent->Velocity = LaunchVector;
	inProjectile->ProjectileComponent->ProjectileGravityScale = GravityScale;
}
