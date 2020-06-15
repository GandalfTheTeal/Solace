// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"

#include "VisionComponent.generated.h" // Always last


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOLACE_API UVisionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVisionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<AActor*> ActorsToUseFromWorld;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

	UFUNCTION(BlueprintCallable, Category = CustomAI)
		TArray<AActor*> GetPawnsInSight(TArray<AActor*> ActorsToUse, float VisionAngle, float VisionDistance, bool bShowRay, bool bShowData);		// Returns an array of all pawns within a set angle in front of the position

	UFUNCTION(BlueprintCallable, Category = CustomAI)
		AActor* GetNearestInSight(TArray<AActor*> ActorsToUse, TSubclassOf<AActor> ClassToFind);

	UFUNCTION()
		double CalculateAngleFromFront(float a, float b, float c);
	
};
