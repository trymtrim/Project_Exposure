//Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

AMainPlayerController::AMainPlayerController ()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = false;
	bEnableTouchOverEvents = false;
	bEnableMouseOverEvents = false;
}

void AMainPlayerController::PlayerTick (float deltaTime)
{
	Super::PlayerTick (deltaTime);
}

void AMainPlayerController::SetupInputComponent ()
{
	//Set up gameplay key bindings
	Super::SetupInputComponent ();
}
