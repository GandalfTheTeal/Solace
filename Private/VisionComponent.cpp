// Fill out your copyright notice in the Description page of Project Settings.

#include "VisionComponent.h"

#include <math.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

#include "Engine.h"


// Sets default values for this component's properties
UVisionComponent::UVisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVisionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Gets every actor of pre defined array in front of this components parent actor
TArray<AActor*> UVisionComponent::GetPawnsInSight(TArray<AActor*> ActorsToUse, float VisionAngle, float VisionDistance, bool bShowRay, bool bShowData)
{

	if (ActorsToUse.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("GetPawnsInSight input invalid"));
		return ActorsToUse;
	}

	TArray<AActor*> actorsInSight;
	AActor* owningActor = GetOwner();
	FVector currentLocation = owningActor->GetActorLocation(); // Your location
	FVector othersLocation; // Pawn in world

	for (int i = 0; i < ActorsToUse.Num(); i++)
	{
		if (ActorsToUse[i] && ActorsToUse[i] != owningActor && !ActorsToUse[i]->IsPendingKill())
		{
			othersLocation = ActorsToUse[i]->GetActorLocation();

			if ((FVector::Dist(othersLocation, currentLocation)) < VisionDistance)
			{
				float distToEnemy = FVector::Dist2D(othersLocation, currentLocation);
				float distToPoint = distToEnemy;
				FVector newPoint = owningActor->GetActorForwardVector() * distToEnemy;
				newPoint += owningActor->GetActorLocation();
				float distFromPointToEnemy = FVector::Dist2D(othersLocation, newPoint);

				if (GetWorld() && bShowRay)
				{
					DrawDebugLine(GetWorld(), othersLocation, currentLocation, FColor::Green, false, 0.15f, 100, 5.0f);
					DrawDebugLine(GetWorld(), newPoint, currentLocation, FColor::Purple, false, 0.15f, 100, 5.0f);
					DrawDebugLine(GetWorld(), newPoint, othersLocation, FColor::Orange, false, 0.15f, 100, 5.0f);
				}

				float Angle = CalculateAngleFromFront(distToEnemy, distToPoint, distFromPointToEnemy);

				if (Angle <= VisionAngle)
				{
					if (GetWorld())
					{
						FHitResult ActorHitResults;

						FCollisionObjectQueryParams CollisionParams;
						CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);

						FVector traceFrom = owningActor->GetActorLocation();
						traceFrom.Z += 150;

						GetWorld()->LineTraceSingleByObjectType(ActorHitResults, traceFrom, othersLocation, CollisionParams);
						//GetWorld()->LineTraceSingleByChannel(ActorHitResults, traceFrom, othersLocation, ECollisionChannel::ECC_WorldStatic);
						/*if (ActorHitResults.Actor.Get())
						{
							GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *ActorHitResults.Actor.Get()->GetName()));
						}*/

						//if(GetWorld()->LineTraceSingleByObjectType(ActorHitResults, traceFrom, othersLocation, CollisionParams) == false)
						if (!ActorHitResults.Actor.Get())
						{
							actorsInSight.AddUnique(ActorsToUse[i]);

							if (GEngine && bShowData)
							{
								GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("a = %f | b = %f | c = %f | angle = %f"), distToEnemy, distToPoint, distFromPointToEnemy, Angle));
								GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("x = %f | y = %f | z = %f"), newPoint.X, newPoint.Y, newPoint.Z));
							}
						}
					}
				}
			}
		}
	}

	return actorsInSight;
	return ActorsToUse; // empty array
}

// Gets the closest actor in front of this components parent actor
AActor * UVisionComponent::GetNearestInSight(TArray<AActor*> ActorsToUse, TSubclassOf<AActor> ClassToFind)
{
	FVector myCurrentLocation = GetComponentLocation();
	float distanceCheck = (float)INT64_MAX;
	AActor* closestActor = nullptr;

	for (int32 i = 0; i < ActorsToUse.Num(); i++)
	{
		if (ActorsToUse[i] != nullptr && !ActorsToUse[i]->IsPendingKill() && ActorsToUse[i]->IsA(ClassToFind))
		{
			float distance = FVector::Dist2D(ActorsToUse[i]->GetActorLocation(), myCurrentLocation);
			if (distance < distanceCheck)
			{
				closestActor = ActorsToUse[i];
				distanceCheck = distance;
			}
		}
	}
	return closestActor;
}

// Returns the angle between the forward vector of this components parent actor and the position of the other actor
double UVisionComponent::CalculateAngleFromFront(float a, float b, float c) // a is character to enemy, b is character to point created, c is point created to enemy
{
	double cosOfC = ((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));
	double angle = acos(cosOfC);

	angle = angle * 180 / PI;
	return angle;
}