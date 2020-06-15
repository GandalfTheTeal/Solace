// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner_Base.generated.h"

class ASpawnLocation_Base;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemiesKilledSignature);

UCLASS()
class SOLACE_API AEnemySpawner_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(BlueprintAssignable, Category = "EnemySpawnHooks")
		FEnemiesKilledSignature EnemiesAllKilledDelegate;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<ASpawnLocation_Base*> LocationsInRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float range = 1000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int NumEnemiesSpawned;

public:

	UFUNCTION(BlueprintCallable)
		void SpawnEnemies(int wave);

	UFUNCTION(BlueprintCallable)
		void DecrementNumSpawned();
	
protected:

	void GetPointsInRadius();
};
