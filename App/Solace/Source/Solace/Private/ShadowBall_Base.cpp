// Fill out your copyright notice in the Description page of Project Settings.

#include "ShadowBall_Base.h"

// Sets default values
AShadowBall_Base::AShadowBall_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShadowBall_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShadowBall_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

