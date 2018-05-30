//Fill out your copyright notice in the Description page of Project Settings.

#include "SimulationGameController.h"
#include "Engine/World.h"
#include "CustomGameViewportClient.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

//Sets default values
ASimulationGameController::ASimulationGameController ()
{
 	//Set this pawn to call Tick () every frame.
	PrimaryActorTick.bCanEverTick = true;
}

//Called when the game starts or when spawned
void ASimulationGameController::BeginPlay ()
{
	Super::BeginPlay ();

	//Setup camera
	_defaultPosition = GetActorLocation ();
	_defaultRotation = FVector (0.0f, 324.0f, 135.0f);

	_cameraMovement = new CameraMovement (this);

	//Setup pawns
	_drillPawn->SetGameController (this);

	//Enable menu UI
	_uiController->Enable (_uiController->menuRef, 1);
	//Enable currentTurn UI
	_uiController->Enable (_uiController->currentTurnRef, 1);
}

//Called every frame
void ASimulationGameController::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);

	if (FVector::Distance (GetActorLocation (), _cameraMovement->GetTargetPosition ()) > 5.0f)
		_cameraMovement->Update (DeltaTime);
	else if (!_miniGameIsActive && FVector::Distance (GetActorLocation (), _defaultPosition) > 50)
	{
		switch (_miniGameActive)
		{
		case 1:
			GetWorld ()->GetFirstPlayerController ()->Possess (_minePawn);
			_minePawn->setup ();
			break;
		case 2:
			
			break;
		case 3:
			GetWorld ()->GetFirstPlayerController ()->Possess (_drillPawn);
			_drillPawn->StartGame ();
			//_drillPawn->SetActorLocation (GetActorLocation ());
			break;
		}

		_miniGameIsActive = true;
	}

	if (fadingIn || fadingOut)
		UpdateFading (DeltaTime);

		//As long as we are in the simulation run the day/night cycle
	if (_simulationRunning)
		UpdateCycle ();
}

void ASimulationGameController::SpawnUnit (int index)
{
	UWorld* world = GetWorld ();
	
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
		//spawnParams.Owner = this;
		
		FVector spawnPosition = GetActorLocation ();
		FRotator rotator = FVector (0.0f, 0.0f, 0.0f).Rotation ();

		//Spawn the unit
		_controlledUnit = world->SpawnActor <APlaceableUnit> (controlledUnit, spawnPosition, rotator, spawnParams);
	}
}

void ASimulationGameController::PlaceUnit ()
{
	if (_controlledUnit->PlaceUnit ())
	{
		int miniGameType = _controlledUnit->GetTypeIndex ();

		_simulation->OnPlaceUnit (miniGameType);

		_miniGameActive = miniGameType;

		_controlledUnit = nullptr;

		//When the unit is placed, start simulation
		StartSimulation ();
	}
}

void ASimulationGameController::StartSimulation ()
{
	//UN-COMMENT THIS TO ENABLE MINIGAMES

	//Disable simulation UI
	_uiController->Disable (_uiController->simulationRef);
	//Enable simulationTest UI
	_uiController->Enable (_uiController->simulationTestRef, 0);

	_simulationRunning = true;
}

void ASimulationGameController::StopSimulation ()
{
	//Disable simulationTest UI
	_uiController->Disable (_uiController->simulationTestRef);

	if (!_miniGamesOn)
	{
		StartNewTurn ();
		_simulationRunning = false;
		return;
	}

	//If it's time for a new minigame, enter that, otherwise start new turn
	if (_currentTurn == 1 || _currentTurn == 4 || _currentTurn == 7)
		EnterMiniGame ();
	else
		StartNewTurn ();

	_simulationRunning = false;
}

void ASimulationGameController::EnterMiniGame ()
{
	switch (_miniGameActive)
	{
	case 1:
		_cameraMovement->MoveTo (_minePosition, _mineRotation);
		break;
	case 2:
			ExitMiniGame ();
		break;
	case 3:
		_cameraMovement->MoveTo (_drillPosition, _drillRotation);
		FadeIn (0.75f, 0.5f);
		FadeOut (3.5f, 1.0f);
		break;
	}

	_firstPlayed = _miniGameActive;
}

void ASimulationGameController::ExitMiniGame ()
{
	GetWorld ()->GetFirstPlayerController ()->Possess (this);

	_miniGameIsActive = false;

	_cameraMovement->MoveTo (_defaultPosition, _defaultRotation);

	//After exiting minigame, start a new turn
	//Maybe run this a little later
	StartNewTurn ();

	_miniGameActive = 0;
}

void ASimulationGameController::StartNewTurn ()
{
	_currentTurn++;
	currentTurnText = "Turn " + FString::FromInt (_currentTurn);

	//Enable simulation UI
	_uiController->Enable (_uiController->simulationRef, 0);

	_simulation->OnNewTurn (_currentTurn);
}

void ASimulationGameController::FadeIn (float delayTime, float fadeTime)
{
	fadeInTimer = delayTime;
	fadeInDelay = fadeTime;
	fadingIn = true;
}

void ASimulationGameController::FadeOut (float delayTime, float fadeTime)
{
	fadeOutTimer = delayTime;
	fadeOutDelay = fadeTime;
	fadingOut = true;
}

void ASimulationGameController::UpdateFading (float deltaTime)
{
	if (fadingIn)
	{
		fadeInTimer -= deltaTime;

		if (fadeInTimer < 0.0f)
		{
			UCustomGameViewportClient* GameViewportClient = Cast <UCustomGameViewportClient> (GetWorld ()->GetGameViewport ());

			if (GameViewportClient)
			{
				GameViewportClient->Fade (fadeInDelay, true);
				fadingIn = false;
			}
		}
	}

	if (fadingOut)
	{
		fadeOutTimer -= deltaTime;

		if (fadeOutTimer < 0.0f)
		{
			UCustomGameViewportClient* GameViewportClient = Cast <UCustomGameViewportClient> (GetWorld ()->GetGameViewport ());

			if (GameViewportClient)
			{
				GameViewportClient->Fade (fadeOutDelay, false);
				fadingOut = false;
			}
		}
	}
}

void ASimulationGameController::CheckAFK ()
{

}

void ASimulationGameController::OnSpacePress ()
{
	if (_simulationRunning)
		StopSimulation ();
}

void ASimulationGameController::OnMouseClick ()
{
	//If the player currently controls a unit, place it
	if (_controlledUnit)
		PlaceUnit ();
	//If game hasn't started, start it
	else if (!gameStarted)
	{
		gameStarted = true;

		//Enable simulation UI
		_uiController->Enable (_uiController->simulationRef, 0);

		//Enable resources UI
		_uiController->Enable (_uiController->resourcesRef, 0);

		//Disable menu UI
		_uiController->Disable (_uiController->menuRef);
	}
}

//Called to bind functionality to input
void ASimulationGameController::SetupPlayerInputComponent (UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent (PlayerInputComponent);

	PlayerInputComponent->BindAction ("MouseClick", IE_Pressed, this, &ASimulationGameController::OnMouseClick);
	PlayerInputComponent->BindAction ("Space", IE_Pressed, this, &ASimulationGameController::OnSpacePress);
}
