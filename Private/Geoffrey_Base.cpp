// Fill out your copyright notice in the Description page of Project Settings.

#include "Geoffrey_Base.h"
#include "Engine.h"

#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"

#include "GuardPoint_Base.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "LauncherComponent.h"

// Sets default values
AGeoffrey_Base::AGeoffrey_Base()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	SetRootComponent(Collider);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);

	MyLauncher = CreateDefaultSubobject<ULauncherComponent>("MyLauncher");
	MyLauncher->AttachTo(RootComponent);

}

// Called when the game starts or when spawned                                                                                                                                                   
void AGeoffrey_Base::BeginPlay()
{
	Super::BeginPlay();

	Delay(0.25f);

	if (GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		Target = GetWorld()->GetFirstPlayerController()->GetPawn();
	}

	InitialRotation = GetActorRotation();
}

// Called every frame
void AGeoffrey_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldRotate)
	{
		RotateTowardsTarget();
	}
}

// Called to bind functionality to input
void AGeoffrey_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AGeoffrey_Base::TakeDamage_Implementation(float inDamage)
{
	if (bCanTakeDamage == false)
	{
		return Health;
	}

	if (Health <= 0) { return 0.0f; }
	Health -= inDamage;

	if (Health <= 0)
	{
		BreakPiece();
		Plates--;
		Health = MaxHealth;
	}

	return Health;
}

void AGeoffrey_Base::BreakPiece_Implementation()
{

}

void AGeoffrey_Base::Slam_Implementation()
{
}

void AGeoffrey_Base::Punch_Implementation()
{
}

void AGeoffrey_Base::LandSlide_Implementation()
{
}

void AGeoffrey_Base::Earthquake_Implementation()
{
}

void AGeoffrey_Base::Activate(AActor * inTarget)
{
}

void AGeoffrey_Base::OnCrystalDestroyed_Implementation()
{
}

void AGeoffrey_Base::ChooseAttack()
{
	if (Target == nullptr) { return; }

	if (bIsStunned) { return; }

	if (GetTargetOnGround() == true)
	{
		Slam();
	}
	else
	{
		Punch();
	}
}

bool AGeoffrey_Base::GetTargetOnGround()
{
	if (Target == nullptr) { return false; }

	float height = Target->GetActorLocation().Z - FloorHeight;

	DistFromFloor = height;

	if (height < OnFloorDist)
	{
		return true;
	}
	return false;
}

bool AGeoffrey_Base::ShouldSlam()
{

	return true;
}

void AGeoffrey_Base::FindFloorHeight()
{
	TArray<AActor*> outActors;

	UGameplayStatics::GetAllActorsOfClass(this, AGuardPoint_Base::StaticClass(), outActors);

	if (outActors.Num() <= 0) { return; }

	float closest = INT32_MAX;

	AActor* closestActor = nullptr;

	for (int i = 0; i < outActors.Num(); i++)
	{
		float dist = FVector::Dist(GetActorLocation(), outActors[i]->GetActorLocation());

		if (dist < closest)
		{
			closest = dist;
			closestActor = outActors[i];
		}
	}
	FloorHeight = closestActor->GetActorLocation().Z;
}

void AGeoffrey_Base::GetLandslideColliders()
{
}

ESectors AGeoffrey_Base::GetSector()
{
	float dist = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());

	//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("dist = %f"), dist)); }

	int num = GetNearMidFar(dist);
	float LeftRightDist = Target->GetActorLocation().Y - GetActorLocation().Y;
	if (LeftRightDist < CentreDist && LeftRightDist > -CentreDist)
	{
		return ESectors::Centre;
	}
	if (LeftRightDist > 0)
	{
		switch (num)
		{
		case 0:
			return ESectors::FrontRight_Near;

		case 1:
			return ESectors::FrontRight_Mid;

		case 2:
			return ESectors::FrontRight_Far;

		default:
			break;
		}
	}
	else
	{
		switch (num)
		{
		case 0:
			return ESectors::FrontLeft_Near;

		case 1:
			return ESectors::FrontLeft_Mid;

		case 2:
			return ESectors::FrontLeft_Far;

		default:
			break;
		}
	}
	return ESectors::Centre;
}

void AGeoffrey_Base::RotateTowardsTarget()
{
	if (Target == nullptr) { return; }

	FRotator CurrentRotation = GetActorRotation();
	float ZRotation = GetActorRotation().Yaw - InitialRotation.Yaw;

	if (bIsStunned == false)
	{
		Angle = CalculateAngleFromFront();
	}

	if (Angle > 0.0f && Angle > AcceptanceAngle && ZRotation < MaxZRotation)
	{
		// Turn Right
		FRotator newRot = GetActorRotation();
		newRot += FRotator(0.0f, RotSpeed, 0.0f);
		SetActorRotation(newRot);
	}
	else if(Angle < 0.0f && Angle < AcceptanceAngle && ZRotation > -MaxZRotation)
	{
		// Turn Left
		FRotator newRot = GetActorRotation();
		newRot -= FRotator(0.0f, RotSpeed, 0.0f);
		SetActorRotation(newRot);
	}
}

float AGeoffrey_Base::CalculateAngleFromFront()
{
	float distToEnemy = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());
	float distToPoint = distToEnemy;
	FVector newPoint = GetActorForwardVector() * distToEnemy;
	newPoint += GetActorLocation();
	float distFromPointToEnemy = FVector::Dist2D(Target->GetActorLocation(), newPoint);

	float a = distToEnemy;
	float b = distToPoint;
	float c = distFromPointToEnemy;

	double cosOfC = ((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));
	double Angle = acos(cosOfC);

	Angle = Angle * 180 / PI;

	if (GetIsRight())
	{
		return Angle;
	}
	else
	{
		return -Angle;
	}
}

bool AGeoffrey_Base::GetIsRight()
{
	float LeftDist = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
	float RightDist = FVector::Dist(GetActorLocation() + GetActorRightVector(), Target->GetActorLocation());

	if (LeftDist > RightDist)
	{
		return true;
	}
	return false;
}

int AGeoffrey_Base::GetNearMidFar(float inDist)
{
	if (inDist > FarDist)
	{
		return 2;
	}
	else if (inDist > MidDist)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void AGeoffrey_Base::Delay(float time)
{
	GetWorld()->GetTimerManager().SetTimer(EmptyTimerHandle, this, &AGeoffrey_Base::FindFloorHeight, time, false);
}
