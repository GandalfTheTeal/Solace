// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "SolaceCharacter.generated.h"

class AAICharacter_Base;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathSignature);

UCLASS(config=Game)
class ASolaceCharacter : public ACharacter
{
	GENERATED_BODY()

	// Camera boom positioning the camera behind the character /
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Follow camera 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float MaxHealth;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Detection)
		bool bIsMakingNoise;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
		bool bIsDead;

	/*How many melee enemies can attack at once*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ticketing")
		int MaxMeleeTickets = 3;

	/*How many ranged enemies can attack at once*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ticketing")
		int MaxRangedTickets = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ticketing")
		int numWaitingRangedEnemies = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ticketing")
		int numWaitingMeleeEnemies = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ticketing")
		bool bIsInCombat = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
		bool bIsInvulnerable;

private:

	UPROPERTY(VisibleAnywhere, Category = "Ticketing")
		TArray<AAICharacter_Base*> RangedTicketedEnemies;

	UPROPERTY(VisibleAnywhere, Category = "Ticketing")
		int RangedTicketsUsed;

	UPROPERTY(VisibleAnywhere, Category = "Ticketing")
		TArray<AAICharacter_Base*> MeleeTicketedEnemies;

	UPROPERTY(VisibleAnywhere, Category = "Ticketing")
		int MeleeTicketsUsed;

		class TDoubleLinkedList<AAICharacter_Base*> WaitingRangedEnemies;

		class TDoubleLinkedList<AAICharacter_Base*> WaitingMeleeEnemies;

	UPROPERTY()
		AAICharacter_Base* TempSpot;

public:

	/* Damage Types are : Water, Geoffrey, Fall | Anything else is considered normal */
	UFUNCTION(BlueprintCallable, Category = "Health")
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, FString DamageType);

	UFUNCTION(BlueprintCallable, Category = "Health")
		float IncreaseMaxHealth(float HealthIncrease);

	UFUNCTION(BlueprintCallable, Category = "Detection")
		bool GetIsMakingNoise();

	UPROPERTY(BlueprintAssignable, Category = "Health")
		FDeathSignature DeathDelegate;

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnDeath();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnFallDeath();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnWaterDeath();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnTakeDamage();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnWaterDamage();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnGeoffreyDamage();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnHeal();

	UFUNCTION(BlueprintNativeEvent, Category = "Ticketing")
		void OnTicketAdded();

	UFUNCTION(BlueprintNativeEvent, Category = "Ticketing")
		void OnAllTicketsRemoved();

	/* Called from enemy to add itself to the waiting list */
	UFUNCTION(BlueprintCallable, Category = "Ticketing")
		bool TryAddTicket(bool bIsRanged, AAICharacter_Base* Attacker);

	/* Called from the enemy to remove itself from its respective list */
	UFUNCTION(BlueprintCallable, Category = "Ticketing")
		bool RemoveTicket(bool bIsRanged, bool bHasDied, AAICharacter_Base* Attacker);

protected:

	virtual void Die();

private:

	/* Called when an enemy removes itself from either active lists to choose the next enemy allowed to attack */
	UFUNCTION()
		void ChooseTickets(bool bIsRanged);

public:
	ASolaceCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void Exit();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/*Returns health*/
	float GetHealth() { return Health; }

	bool GetIsDead() { return bIsDead; }

	UFUNCTION(BlueprintCallable)
		void SetInvulnerability(bool CanTakeDamage) { bIsInvulnerable = CanTakeDamage; }
};

