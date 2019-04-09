// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hurtable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHurtable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOLACE_API IHurtable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
//protected:
//
//	UPROPERTY(EditAnywhere, Category = "Health")
//		float CurrentHealth = 10;
//
//	UPROPERTY(EditAnywhere, Category = "Health")
//		float MaxHealth = 20;
	
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
		float TakeDamage(float Damage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
		float Heal(float HealthIncrease);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
		float IncreaseMaxHealth(float HealthIncrease);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
		void Die();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
		float GetHealth();
};
