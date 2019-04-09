// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnLocation_Base.h"

#include "EnemySpawner_Base.h"
#include "AICharacter_Base.h"

// Sets default values
ASpawnLocation_Base::ASpawnLocation_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnLocation_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnLocation_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnLocation_Base::SpawnEnemy_Implementation(AEnemySpawner_Base* SpawnedFrom, int wave)
{
}

void ASpawnLocation_Base::SetVile_Implementation(AAICharacter_Base* Vile)
{
	if (Vile != nullptr)
	{
		Vile->Health = HealthVile;
		Vile->Damage = DamageVile;
		Vile->AttackCooldown = AttackCooldownVile;
		Vile->MaxChaseSpeed = SpeedVile;
		Vile->TicketTime = TicketTimeVile;
	}
}

void ASpawnLocation_Base::SetReaver_Implementation(AAICharacter_Base* Reaver)
{
	if (Reaver != nullptr)
	{
		Reaver->Health = HealthReaver;
		Reaver->Damage = DamageReaver;
		Reaver->AttackCooldown = AttackCooldownReaver;
		Reaver->MaxChaseSpeed = SpeedReaver;
		Reaver->TicketTime = TicketTimeReaver;
	}
}

//float AttackCooldown = 2.0f;
//float SpeedReaver = 400.0f;
//float HealthVile = 5.0f;
//float DamageVile = 0.0f;
//float AttackCooldownVile = 2.0f;
//ticket time