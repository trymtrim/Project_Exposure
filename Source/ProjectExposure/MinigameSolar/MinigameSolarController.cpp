//Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameSolarController.h"
#include "SimulationGameController.h"
#include "Engine/World.h"
#include "UIController.h"

//Sets default values
AMinigameSolarController::AMinigameSolarController ()
{
 	//Set this pawn to call Tick () every frame
	PrimaryActorTick.bCanEverTick = true;
}

//Called when the game starts or when spawned
void AMinigameSolarController::BeginPlay ()
{
	Super::BeginPlay ();

	SetActorTickEnabled (false);
}

//Called every frame
void AMinigameSolarController::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);
}

void AMinigameSolarController::InitializeGrid ()
{
	/*UWorld* world = GetWorld ();
	
	if (world)
	{
		if (_controlledUnit)
			_controlledUnit->Destroy ();

		//Define the unit to spawn
		TSubclassOf <APlaceableUnit> controlledUnit;
		
		switch (index)
		{
		case 0:
			controlledUnit = _nuclearReactorPrefab;
			break;
		case 1:
			controlledUnit = _windmillPrefab;
			break;
		case 2:
			controlledUnit = _oilRigPrefab;
			break;
		}
		
		FActorSpawnParameters spawnParams;
		FVector spawnPosition = GetActorLocation ();
		FRotator rotator = FVector (0.0f, 0.0f, 0.0f).Rotation ();

		//Spawn the unit
		_controlledUnit = world->SpawnActor <APlaceableUnit> (controlledUnit, spawnPosition, rotator, spawnParams);

		//Disable simulation UI
		_uiController->Disable (1);
	}*/
}

void AMinigameSolarController::SetGameController (ASimulationGameController* gameController)
{
	_gameController = gameController;
}

void AMinigameSolarController::StartGame ()
{
	SetActorTickEnabled (true);

	SetScore (0);

	_endPanelShown = false;
}

void AMinigameSolarController::StartGamePlay ()
{
	_gameStarted = true;

	//Enable solarMiniGame UI
	_uiController->Enable (20, 0);

	EndGame ();
}

void AMinigameSolarController::EndGame ()
{
	SetActorTickEnabled (false);

	SetScore (500);
	endScoreText = "You got " + FString::FromInt (_score) + " out of 3000 points";

	//Disable solarMiniGame UI
	_uiController->Disable (20);

	//Enable lose panel UI
	//_uiController->Enable (18, 2);
	//Enable win panel UI
	_uiController->Enable (19, 2);

	EndScreen ();
	
	_endPanelShown = true;
	_gameStarted = false;
}

void AMinigameSolarController::SetScore (int score)
{
	_score = score;

	scoreText = FString::FromInt (_score);

	Update ();
}

void AMinigameSolarController::GoBackToSimulation ()
{
	//Disable lose panel UI
	//_uiController->Disable (18);
	//Disable win panel UI
	_uiController->Disable (19);

	_gameController->ExitMiniGame ();
}

void AMinigameSolarController::OnMouseClick ()
{
	if (_endPanelShown)
		GoBackToSimulation ();
	else if (!_gameStarted)
		StartGamePlay ();
}

//Called to bind functionality to input
void AMinigameSolarController::SetupPlayerInputComponent (UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent (PlayerInputComponent);

	PlayerInputComponent->BindAction ("MouseClick", IE_Pressed, this, &AMinigameSolarController::OnMouseClick);
}
