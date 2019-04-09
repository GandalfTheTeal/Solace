// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SolaceCharacter.h"
#include "PlayerBase.generated.h"

/**
 * 
 */
UCLASS()
class SOLACE_API APlayerBase : public ASolaceCharacter
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Light)
	bool bInLight;
	
public:

	bool GetIsInLight() { return bInLight; }
};
