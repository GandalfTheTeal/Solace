// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnLocation_Base.generated.h"

class AAICharacter_Base;
class AEnemySpawner_Base;

UCLASS()
class SOLACE_API ASpawnLocation_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnLocation_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff")
		TArray<TSubclassOf<AAICharacter_Base>> AIClassesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff")
		bool bShouldGuard;

#pragma region Reaver
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff Reaver")
		float HealthReaver = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff Reaver")
		float DamageReaver = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff Reaver")
		float AttackCooldownReaver = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff Reaver")
		float SpeedReaver = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff Reaver")
		float TicketTimeReaver = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff Reaver")
		float TicketCooldownReaver = 5.0f;
#pragma endregion

#pragma region Vile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff Vile")
		float HealthVile = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff Vile")
		float DamageVile = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff Vile")
		float AttackCooldownVile = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff Vile")
		float SpeedVile = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff Vile")
		float TicketTimeVile = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Stuff Vile")
		float TicketCooldownVile = 5.0f;
#pragma endregion

public:

	UFUNCTION(BlueprintNativeEvent)
		void SpawnEnemy(AEnemySpawner_Base* SpawnedFrom, int wave);

	int GetWaveNum() { return AIClassesToSpawn.Num(); }

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetVile(AAICharacter_Base* Vile);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetReaver(AAICharacter_Base* Reaver);
};
