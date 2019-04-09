// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AITargetable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAITargetable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOLACE_API IAITargetable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

	/*UPROPERTY()
		int RangedTickets = 1;
	UPROPERTY()
		int RangedTicketsUsed;
	UPROPERTY()
		int MeleeTickets = 2;
	UPROPERTY()
		int MeleeTicketsUsed;*/


public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ticketing")
		bool AddTicket(bool bIsRanged);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ticketing")
		void RemoveTicket(bool bIsRanged);

};
