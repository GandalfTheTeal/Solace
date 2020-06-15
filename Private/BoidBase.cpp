// Fill out your copyright notice in the Description page of Project Settings.

#include "BoidBase.h"

#pragma optimize("", off)

// Sets default values
ABoidBase::ABoidBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoidBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoidBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyRotation = GetActorRotation();
	MyLocation = GetActorLocation();
	ForwardVector = GetActorForwardVector();
	AlterCourse();
	Move(DeltaTime);
}

void ABoidBase::Move(float DeltaTime)
{
	Speed /= 2;
	if (Speed < 0) { Speed = 50; }
	Speed = 50 + Speed * MaxSpeed;
	SetActorLocation(GetActorLocation() + (GetActorForwardVector() * Speed * DeltaTime));
	Speed = 0;
}

void ABoidBase::AlterCourse()
{
	GetCohesion();
	GetAlignment();
	GetSeparation();
	CohesionDir *= Cohesion;
	SeparationDir *= Separation;
	TempRot = (CohesionDir + SeparationDir);
	//TempRot.Pitch /= 2;
	TempRot.Normalize();
	//FVector temp = (CohesionDir * Cohesion) + (SeparationDir * Separation) + (AlignmentDir * Alignment);
	FRotator temp = FRotator(TempRot.Pitch, TempRot.Yaw, 0) + (AlignmentDir * Alignment);
	temp.Pitch /= 2;
	temp.Yaw /= 2;
	temp.Normalize();
	temp.Roll = 0;

	NewRotation = GetActorRotation() + (temp * TurnSpeed);
	//FRotator newRot = FRotator(temp.Y, temp.Z, temp.X);

	SetActorRotation(NewRotation);
	//SetActorRotation(GetActorRotation() + (newRot * TurnSpeed));
}

void ABoidBase::GetCohesion()
{
	int num = 0;
	float TempFloat = 0.0f;
	for (int i = 0; i < BoidsNearMe.Num(); i++)
	{
		TempDist = FVector::Dist(BoidsNearMe[i]->GetActorLocation(), GetActorLocation());

		if (TempDist < (5000 - 0.1f) && TempDist - 100 != 0)
		{
			TempVector = (BoidsNearMe[i]->GetActorLocation() - GetActorLocation());
			TempVector.Normalize();
			TempVector -= ForwardVector;
			//TempVector *= TempDist - 100;
			TempFloat = ((2000000) / (-TempDist + 5000) - 400);
			TempVector *= TempFloat;
			TempDir += TempVector;
			num++;
		}
	}
	TempDir /= num;
	CohesionDir = FRotator(TempDir.Z, TempDir.Y, 0);
	TempDir.Normalize();
	Speed += TempDir.X;
	TempDir = FVector::ZeroVector;
}

void ABoidBase::GetSeparation()
{
	int num = 0;
	for (int i = 0; i < BoidsNearMe.Num(); i++)
	{
		TempDist = FVector::Dist(BoidsNearMe[i]->GetActorLocation(), GetActorLocation());

		if (TempDist < GroupRadius && TempDist > 0 && TempDist != 0)
		{
			TempVector = (MyLocation - BoidsNearMe[i]->GetActorLocation());
			TempVector.Normalize();
			TempVector -= ForwardVector;
			TempVector *= ((1000 * 200) / (TempDist)) - 500.0f;
			TempDir += TempVector;
			num++;
		}
	}
	TempDir /= num;
	SeparationDir = FRotator(TempDir.Z, TempDir.Y, TempDir.X);
	TempDir.Normalize();
	Speed += TempDir.X;
	TempDir = FVector::ZeroVector;
}
#pragma optimize("", on)

void ABoidBase::GetAlignment()
{
	int num = 0;
	for (int i = 0; i < BoidsNearMe.Num(); i++)
	{
		if (BoidsNearMe[i] != this)
		{
			TempDist = FVector::Dist(BoidsNearMe[i]->GetActorLocation(), GetActorLocation());

			if (TempDist < GroupRadius && TempDist != 0)
			{
				TempRot = BoidsNearMe[i]->GetActorRotation();
				TempRot -= MyRotation;
				TempDir += FVector(TempRot.Roll, TempRot.Pitch, TempRot.Yaw);
				num++;
			}
		}
	}
	TempDir /= num;
	AlignmentDir = FRotator(TempDir.Y, TempDir.Z, 0);
	AlignmentDir.Normalize();
	TempDir = FVector::ZeroVector;
}