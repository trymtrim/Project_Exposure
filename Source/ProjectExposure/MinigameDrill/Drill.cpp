//Fill out your copyright notice in the Description page of Project Settings.

#include "Drill.h"

//Sets default values
ADrill::ADrill ()
{
 	//Set this actor to call Tick () every frame.
	PrimaryActorTick.bCanEverTick = true;
}

//Called when the game starts or when spawned
void ADrill::BeginPlay ()
{
	Super::BeginPlay ();
}

//Called every frame
void ADrill::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);
}
