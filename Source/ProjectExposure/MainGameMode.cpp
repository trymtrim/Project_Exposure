//Fill out your copyright notice in the Description page of Project Settings.

#include "MainGameMode.h"
#include "MainPlayerController.h"
#include "PlaceableUnit.h"
#include "UObject/ConstructorHelpers.h"

AMainGameMode::AMainGameMode ()
{
	//Use our custom PlayerController class
	PlayerControllerClass = AMainPlayerController::StaticClass ();
	
	//Set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder <APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));

	if (PlayerPawnBPClass.Class != NULL)
		DefaultPawnClass = PlayerPawnBPClass.Class;*/
}
