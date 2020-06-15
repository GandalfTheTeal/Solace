// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidBase.generated.h"

UCLASS()
class SOLACE_API ABoidBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	UPROPERTY()
		FRotator TempRot;

	UPROPERTY()
		FVector TempDir = FVector::ZeroVector;

	UPROPERTY()
		FVector TempVector;

	UPROPERTY()
		float TempDist = 0.0f;

	UPROPERTY()
		FRotator NewRotation;

	UPROPERTY()
		FRotator MyRotation;

	UPROPERTY()
		FVector MyLocation;

	UPROPERTY()
		FVector ForwardVector;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		float Speed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MaxSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float TurnSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Debugging")
		TArray<ABoidBase*> BoidsNearMe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Group")
		float GroupRadius = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Group")
		float NearCutoff = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Separation = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		FRotator SeparationDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Alignment = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		FRotator AlignmentDir = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Cohesion = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		FRotator CohesionDir;

protected:

	UFUNCTION()
		void Move(float DeltaTime);

	UFUNCTION()
		void AlterCourse();

	UFUNCTION()
		void GetCohesion();

	UFUNCTION()
		void GetAlignment();

	UFUNCTION()
		void GetSeparation();

};
