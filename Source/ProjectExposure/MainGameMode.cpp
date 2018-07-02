//Fill out your copyright notice in the Description page of Project Settings.

#include "MainGameMode.h"
#include "MainPlayerController.h"
#include "MainGameState.h"

AMainGameMode::AMainGameMode ()
{
	//Use custom PlayerController class
	PlayerControllerClass = AMainPlayerController::StaticClass ();

	//Use custom GameState class
	GameStateClass = AMainGameState::StaticClass ();
}
