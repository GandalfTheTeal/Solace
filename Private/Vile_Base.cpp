// Fill out your copyright notice in the Description page of Project Settings.

#include "Vile_Base.h"

#include "LauncherComponent.h"
#include "PlayerBase.h"

AVile_Base::AVile_Base()
{
	MyLauncherComponent = CreateDefaultSubobject<ULauncherComponent>("MyLauncherComponent");
	MyLauncherComponent->AttachTo(RootComponent);
	bIsRanged = true;
}

FVector AVile_Base::GetVectorToThrow_Implementation(AActor* Target, float ProjectileSpeed, float LeadOffset)
{
	// Get position of projectile spawn point
	FVector myLocation = GetActorLocation();
	myLocation.X = myLocation.X + GetActorForwardVector().X * ProjectileSpawnOffset.X;
	myLocation.Y = myLocation.Y + GetActorForwardVector().Y * ProjectileSpawnOffset.Y;
	myLocation.Z = myLocation.Z + GetActorForwardVector().Z * ProjectileSpawnOffset.Z;

	// Get position and velocity of target actor
	FVector targetLocation = Target->GetActorLocation();
	FVector targetVelocity = Target->GetVelocity();

	// Get time it would take for the projectile to reach current target location
	float time = (FVector::Dist(myLocation, targetLocation) / 100) / (ProjectileSpeed / 100);

	// Get location of target in n seconds if they keep their heading and velocity
	FVector tempLocation = targetLocation + (targetVelocity * time);
	FVector locationToAim = targetLocation;

	for (int i = 0; i < NumLoops; i++)
	{
		// Calculate approximate time it would take for the projectile to reach the new location
		time = (FVector::Dist(myLocation, tempLocation) / 100 ) / (ProjectileSpeed / 100);

		// Get location of the target at n seconds if the continue on their current path
		locationToAim = targetLocation + (targetVelocity * (time));
		tempLocation = locationToAim;
	}

	// Get location of the target at n seconds if the continue on their current path
	targetLocation += (targetVelocity)* (time / LeadOffset);

	// Get the target location in local space
	FVector relativeLocation = (targetLocation - myLocation);
	//relativeLocation.Z = (980 * time) * 100;
	//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("x = %f | y = %f | z = %f"), relativeLocation.X, relativeLocation.Y, relativeLocation.Z)); }

	// Normalize the relative location of the target point
	relativeLocation.Normalize();
	//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("x = %f | y = %f | z = %f"), relativeLocation.X, relativeLocation.Y, relativeLocation.Z)); }
	
	FVector projectileVelocity;
	projectileVelocity.X = relativeLocation.X * ProjectileSpeed;
	projectileVelocity.Y = relativeLocation.Y * ProjectileSpeed;
	projectileVelocity.Z = relativeLocation.Z;
	//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("x = %f | y = %f | z = %f"), projectileVelocity.X, projectileVelocity.Y, projectileVelocity.Z)); }
	
	return relativeLocation;
	//return projectileVelocity;
}
