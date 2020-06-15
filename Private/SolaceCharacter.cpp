// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SolaceCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Core/Public/Containers/List.h"
#include "Engine.h"

#include "EnemyBase.h"

//////////////////////////////////////////////////////////////////////////
// ASolaceCharacter


void ASolaceCharacter::OnDeath_Implementation()
{
	Die();
	DeathDelegate.Broadcast();
}

void ASolaceCharacter::OnFallDeath_Implementation()
{
	Die();
	DeathDelegate.Broadcast();
}

void ASolaceCharacter::OnWaterDeath_Implementation()
{
	Die();
	DeathDelegate.Broadcast();
}

void ASolaceCharacter::OnTakeDamage_Implementation()
{
}

void ASolaceCharacter::OnHeal_Implementation()
{
}

void ASolaceCharacter::OnWaterDamage_Implementation()
{
}

void ASolaceCharacter::OnGeoffreyDamage_Implementation()
{

}

void ASolaceCharacter::OnTicketAdded_Implementation() 
{
	bIsInCombat = true;
}

void ASolaceCharacter::OnAllTicketsRemoved_Implementation() 
{
	bIsInCombat = false;
}

bool ASolaceCharacter::TryAddTicket(bool bIsRanged, AAICharacter_Base* Attacker)
{
	if (Attacker == nullptr || Attacker->IsPendingKill()) { return false; }
	if (RangedTicketedEnemies.Contains(Attacker) || MeleeTicketedEnemies.Contains(Attacker)) { return true; }
	if (WaitingRangedEnemies.Contains(Attacker) || WaitingMeleeEnemies.Contains(Attacker)) { return true; }

	if (bIsRanged && RangedTicketsUsed < MaxRangedTickets)
	{
		RangedTicketedEnemies.AddUnique(Attacker);
		Attacker->SetTicket(true);
		RangedTicketsUsed++;
		if (bIsInCombat == false)
		{
			OnTicketAdded();
		}
		return true;
	}
	else if (bIsRanged == false && MeleeTicketsUsed < MaxMeleeTickets)
	{
		MeleeTicketedEnemies.AddUnique(Attacker);
		Attacker->SetTicket(true);
		MeleeTicketsUsed++;
		if (bIsInCombat == false)
		{
			OnTicketAdded();
		}
		return true;
	}

	if (bIsRanged)
	{
		WaitingRangedEnemies.AddHead(Attacker);
		numWaitingRangedEnemies++;
	}
	else
	{
		WaitingMeleeEnemies.AddHead(Attacker);
		numWaitingMeleeEnemies++;
	}

	if (bIsInCombat == false)
	{
		OnTicketAdded();
	}

	return true;
}

bool ASolaceCharacter::RemoveTicket(bool bIsRanged, bool bHasDied, AAICharacter_Base* Attacker)
{
	if (Attacker == nullptr || Attacker->IsPendingKill()) {
		return false; 
	}

	if (bIsRanged && RangedTicketedEnemies.Num() > 0 && RangedTicketedEnemies.Contains(Attacker))
	{
		RangedTicketedEnemies.Remove(Attacker);
		RangedTicketedEnemies.Shrink();

		// If still alive
		if (bHasDied == false)
		{
			TempSpot = Attacker;
			Attacker->SetTicket(false);
		}

		RangedTicketsUsed--;

		ChooseTickets(bIsRanged);
	}
	else if (bIsRanged == false && MeleeTicketedEnemies.Num() > 0&& MeleeTicketedEnemies.Contains(Attacker))
	{
		MeleeTicketedEnemies.Remove(Attacker);
		MeleeTicketedEnemies.Shrink();

		if (bHasDied == false)
		{
			TempSpot = Attacker;
			Attacker->SetTicket(false);
		}

		MeleeTicketsUsed--;

		ChooseTickets(bIsRanged);
	}

	// If attacker has just died, remove itself from waiting lists
	if (WaitingRangedEnemies.Contains(Attacker))
	{
		WaitingRangedEnemies.RemoveNode(Attacker);
		numWaitingRangedEnemies--;
	}
	if (WaitingMeleeEnemies.Contains(Attacker))
	{
		WaitingMeleeEnemies.RemoveNode(Attacker);
		numWaitingMeleeEnemies--;
	}

	if (RangedTicketsUsed <= 0 && MeleeTicketsUsed <= 0)
	{
		OnAllTicketsRemoved();
	}
	return true;
}

void ASolaceCharacter::ChooseTickets(bool bIsRanged)
{
	// Waiting lists are empty
	if (bIsRanged)
	{
		if (WaitingRangedEnemies.Num() <= 0 && TempSpot->IsValidLowLevel() && TempSpot->IsPendingKill())
		{
			TempSpot->SetTicket(true);
			return;
		}
	}
	else
	{
		if (WaitingMeleeEnemies.Num() <= 0 && TempSpot->IsValidLowLevel() && TempSpot->IsPendingKill())
		{
			TempSpot->SetTicket(true);
			return;
		}
	}
	
	// Ranged
	if (RangedTicketsUsed < MaxRangedTickets && bIsRanged)
	{
		// If not already on the list
		if (WaitingRangedEnemies.Num() > 0 && WaitingRangedEnemies.GetHead()->GetValue() != TempSpot)
		{
			WaitingRangedEnemies.AddHead(TempSpot);
		}
		TempSpot = nullptr;

		if (WaitingRangedEnemies.Num() > 1)
		{
			TempSpot = WaitingRangedEnemies.GetTail()->GetValue();
		}
		else if (WaitingRangedEnemies.Num() > 0)
		{
			TempSpot = WaitingRangedEnemies.GetHead()->GetValue();
		}

		if (TempSpot == nullptr) { return; }

		if (TempSpot->IsValidLowLevel())
		{
			TempSpot->SetTicket(true);
			RangedTicketedEnemies.Add(TempSpot);
			WaitingRangedEnemies.RemoveNode(TempSpot);
			RangedTicketsUsed++;
		}		

		return;
	}

	// Melee
	else if (MeleeTicketsUsed < MaxMeleeTickets && bIsRanged == false)
	{
		// If not already on the list
		if (WaitingMeleeEnemies.Num() > 0 && WaitingMeleeEnemies.GetHead()->GetValue() != TempSpot)
		{
			WaitingMeleeEnemies.AddHead(TempSpot);
		}
		TempSpot = nullptr;

		if (WaitingMeleeEnemies.Num() > 1)
		{
			TempSpot = WaitingMeleeEnemies.GetTail()->GetValue();
		}
		else if (WaitingMeleeEnemies.Num() > 0)
		{
			TempSpot = WaitingMeleeEnemies.GetHead()->GetValue();
		}

		if (TempSpot->IsValidLowLevel() == false) { return; }
		if (TempSpot->IsPendingKill()) { return; }

		if (TempSpot->IsValidLowLevel())
		{
			TempSpot->SetTicket(true);
			MeleeTicketedEnemies.Add(TempSpot);
			WaitingMeleeEnemies.RemoveNode(TempSpot);
			MeleeTicketsUsed++;
		}

		return;
	}
}

void ASolaceCharacter::Die()
{
	bIsDead = true;

	for (int i = 0; i < RangedTicketedEnemies.Num(); i++)
	{
		if (RangedTicketedEnemies[i])
		{
			RangedTicketedEnemies[i]->RemoveTicket();
		}
	}
	for (int i = 0; i < MeleeTicketedEnemies.Num(); i++)
	{
		if (MeleeTicketedEnemies[i])
		{
			MeleeTicketedEnemies[i]->RemoveTicket();
		}
	}
}

ASolaceCharacter::ASolaceCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 0.1f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera/
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASolaceCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASolaceCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASolaceCharacter::MoveRight);

	PlayerInputComponent->BindAction("Esc", IE_Pressed, this, &ASolaceCharacter::Exit);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASolaceCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASolaceCharacter::LookUpAtRate);
}

void ASolaceCharacter::Exit()
{
	APlayerController* player = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), player, EQuitPreference::Quit);
}

void ASolaceCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASolaceCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASolaceCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = GetActorForwardVector();
			//FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (Value < 0)
		{
			AddMovementInput(Direction, Value*0.5f);
		}

		else
		{
			AddMovementInput(Direction, Value);
		}
	}
}

void ASolaceCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FollowCamera->GetRightVector();
			//FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value*0.6f);
	}
}

float ASolaceCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, FString DamageType)
{
	if (bIsDead) { return 0.0f; }
	if (bIsInvulnerable) { return MaxHealth; }


	if (DamageAmount < 0)
	{
		if(Health - DamageAmount <= MaxHealth)
		{
			Health -= DamageAmount;
			OnHeal();
		}
		else
		{
			Health = MaxHealth;
			OnHeal();
		}
	}
	else
	{
		Health -= DamageAmount;
		if (DamageType == "Water")
		{

			OnWaterDamage();
		}
		if (DamageType == "Geoffrey")
		{
			OnGeoffreyDamage();
		}
		if (DamageType == "Fall")
		{
			OnFallDeath();
			return 0.0f;
		}

		if (Health - DamageAmount <= 0)
		{
			Health = 0;

			if (DeathDelegate.IsBound())
			{
				DeathDelegate.Broadcast();
			}

			OnDeath();
			return 0.0f;
		}
		OnTakeDamage();
	}

	return Health;
}

float ASolaceCharacter::IncreaseMaxHealth(float HealthIncrease)
{
	MaxHealth += HealthIncrease;
	Health = MaxHealth;
	return MaxHealth;
}

bool ASolaceCharacter::GetIsMakingNoise() { return bIsMakingNoise; }
