// Fill out your copyright notice in the Description page of Project Settings.

#include "LightBeam_Base.h"


// Sets default values
ALightBeam_Base::ALightBeam_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALightBeam_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightBeam_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

