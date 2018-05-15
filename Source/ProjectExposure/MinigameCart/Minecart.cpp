// Fill out your copyright notice in the Description page of Project Settings.

#include "Minecart.h"


// Sets default values
AMinecart::AMinecart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMinecart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinecart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

