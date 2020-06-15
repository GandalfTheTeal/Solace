// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Vile_Base.generated.h"

class ULauncherComponent;

/**
 * 
 */
UCLASS()
class SOLACE_API AVile_Base : public AEnemyBase
{
	GENERATED_BODY()

public:
	AVile_Base();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CustomComponents)
		ULauncherComponent* MyLauncherComponent;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector ProjectileSpawnOffset = FVector(100.0f, 0.0f, 100.0f);

	UPROPERTY(EditAnywhere)
		int NumLoops = 3;

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		FVector GetVectorToThrow(AActor* Target, float ProjectileSpeed, float LeadOffset);

};
