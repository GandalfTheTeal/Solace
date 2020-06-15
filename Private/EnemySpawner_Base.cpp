// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner_Base.h"

#include "SpawnLocation_Base.h"
#include "EnemyBase.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner_Base::AEnemySpawner_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner_Base::BeginPlay()
{
	Super::BeginPlay();
	
	GetPointsInRadius();
}

// Called every frame
void AEnemySpawner_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner_Base::GetPointsInRadius()
{
	TArray<AActor*> SpawnLocations;
	UGameplayStatics::GetAllActorsOfClass(this, ASpawnLocation_Base::StaticClass(), SpawnLocations);
	for (int i = 0; i < SpawnLocations.Num(); i++)
	{
		if (SpawnLocations[i]->IsA<ASpawnLocation_Base>())
		{
			float dist = FVector::Dist2D(GetActorLocation(), SpawnLocations[i]->GetActorLocation());
			if (dist < range)
			{
				LocationsInRange.AddUnique(Cast<ASpawnLocation_Base>(SpawnLocations[i]));
			}
		}
	}
}

void AEnemySpawner_Base::SpawnEnemies(int wave)
{
	for (int i = 0; i < LocationsInRange.Num(); i++)
	{
		if (LocationsInRange[i])
		{
			if (wave < LocationsInRange[i]->GetWaveNum() && wave >= 0)
			{
				LocationsInRange[i]->SpawnEnemy(this, wave);
				NumEnemiesSpawned++;
			}
		}
	}
}

void AEnemySpawner_Base::DecrementNumSpawned()
{
	NumEnemiesSpawned--;
	if (NumEnemiesSpawned <= 0)
	{
		EnemiesAllKilledDelegate.Broadcast();
	}
}
