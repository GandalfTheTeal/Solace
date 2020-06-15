// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Geoffrey_Base.generated.h"

UENUM(BlueprintType)
enum class ESectors : uint8
{
	Left_Near,
	Left_Mid,
	Left_Far,

	FrontLeft_Near,
	FrontLeft_Mid,
	FrontLeft_Far,

	Centre,

	FrontRight_Near,
	FrontRight_Mid,
	FrontRight_Far,

	Right_Near,
	Right_Mid,
	Right_Far
};

UCLASS()
class SOLACE_API AGeoffrey_Base : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGeoffrey_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	FTimerHandle EmptyTimerHandle;

protected:

#pragma region Components
	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadOnly)
		class UCapsuleComponent* Collider;

	UPROPERTY()
		class UArrowComponent* ArrowComponent;

	UPROPERTY()
		class UVisionComponent* MyVisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		class ULauncherComponent* MyLauncher;
#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
		AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float SlamDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float PunchDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float LandslideDamage = 10.0f;

	/* Time between ending an attack and starting a new one */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float AttackCooldown = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsStunned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		int Crystals = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		int Plates = 2.0f;

	/*Damage it takes to break a plate off*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float MaxHealth = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float Health = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Detection")
		float FloorHeight = INT_MAX;

	/*How high above the nearest guard point Geoffrey considers off the floor*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
		float OnFloorDist = 200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Detection")
		float DistFromFloor = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotation")
		FRotator InitialRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation")
		float MaxZRotation = 20.0f;

	/*In Degrees/Tick*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation")
		float RotSpeed = 0.1f;

	/*Angle at which Geoffrey stops rotating towards the player*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation")
		float AcceptanceAngle = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		float MidDist = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		float FarDist = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		float FrontAngle = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		float CentreAngle = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		float CentreDist = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
		FVector BoulderLaunchOffset = FVector(500.0f, 0.0f, 500.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
		ESectors Sector = ESectors::FrontRight_Mid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotation")
		bool bShouldRotate = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
		float Angle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
		bool bCanTakeDamage;

public:

	UFUNCTION(BlueprintCallable, Category = "Boss initialization")
		void Activate(AActor* inTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
		float TakeDamage(float inDamage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
		void BreakPiece();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnCrystalDestroyed();

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
		void Slam();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
		void Punch();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
		void LandSlide();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
		void Earthquake();

	UFUNCTION(BlueprintCallable)
		void ChooseAttack();

private:

	bool GetTargetOnGround();

	bool ShouldSlam();

	void FindFloorHeight();

	void GetLandslideColliders();

	UFUNCTION(BlueprintCallable)
	ESectors GetSector();

	void RotateTowardsTarget();


	float CalculateAngleFromFront();

	bool GetIsRight();

	int GetNearMidFar(float inDist);

	void Delay(float time);

};
