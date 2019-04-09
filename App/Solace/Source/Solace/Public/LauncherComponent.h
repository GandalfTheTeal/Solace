// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LauncherComponent.generated.h"

class AProjectileBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOLACE_API ULauncherComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULauncherComponent();

private:

	/*Lead multiplier, higher will shoot in front of its target, lower will trail behind its target*/
	UPROPERTY(EditAnywhere, Category = "Prediction")
		float LeadOffset = 1.0f;

	/*How many times it should predict where the target is going. Has exponentially diminishing returns. 0 is no prediction*/
	UPROPERTY(EditAnywhere, Category = "Prediction")
		int AccuracyLoops = 3;

	/*Gravity scale*/
	UPROPERTY(EditAnywhere, Category = "Prediction")
		float GravityScale = 1.0f;

	/*Accuracy | higher is less accurate | NOT IMPLEMENTED*/
	UPROPERTY(EditAnywhere, Category = "Prediction")
		float Accuracy = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Prediction")
		TSubclassOf<AProjectileBase> ProjectileToThrow;

public:
	UFUNCTION(BlueprintCallable, Category = "LauncherMath")
		FVector PredictLaunchVector(AActor* Target, FVector& outEndPoint, float Speed = 1000.0f, FVector StartPosOffset = FVector::ZeroVector, float LaunchObjectRadius = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Launcher")
		AProjectileBase* LaunchProjectile(FVector LaunchVector, FVector LaunchPos, bool bLaunchFromHere);

	UFUNCTION(BlueprintCallable, Category = "Launcher")
		void RedirectProjectile(FVector LaunchVector, AProjectileBase* inProjectile);
	
};
