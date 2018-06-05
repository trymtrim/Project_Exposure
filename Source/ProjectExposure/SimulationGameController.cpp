//Fill out your copyright notice in the Description page of Project Settings.

#include "SimulationGameController.h"
#include "Engine/World.h"
#include "CustomGameViewportClient.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

//Sets default values
ASimulationGameController::ASimulationGameController ()
{
 	//Set this pawn to call Tick () every frame
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

	if (_movingToMine)
		UpdateMovingToMine ();
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

		//Disable simulation UI
		_uiController->Disable (_uiController->simulationRef);
	}
}

void ASimulationGameController::PlaceUnit ()
{
	if (!_controlledUnit)
		return;

	if (_controlledUnit->PlaceUnit ())
	{
		int miniGameType = _controlledUnit->GetTypeIndex ();

		_simulation->OnPlaceUnit (miniGameType);

		_miniGameActive = miniGameType;

		_controlledUnit = nullptr;

		//When the unit is placed, start simulation
		StartSimulation ();

		_placing = false;
	}
	else
	{
		//Enable simulation UI
		_uiController->Enable (_uiController->simulationRef, 0);

		//Remove currently controlled unit
		_controlledUnit->Destroy ();
		_controlledUnit = nullptr;
	}
}

void ASimulationGameController::StartSimulation ()
{
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
	//if (_currentTurn == 1 || _currentTurn == 4 || _currentTurn == 7)
	//MAKE THIS SHIT OPTIONAL
		EnterMiniGame ();
	//else
		//StartNewTurn ();

	_simulationRunning = false;
}

void ASimulationGameController::EnterMiniGame ()
{
	switch (_miniGameActive)
	{
	case 1:
		//_cameraMovement->MoveTo (_minePosition, _mineRotation);
		_cameraMovement->MoveTo (_mineCameraPositions [0], _mineCameraRotations [0]);
		//_mine->SetActorLocation (FVector (-19420, 1268, 2030));
		_movingToMine = true;
		break;
	case 2:
		ExitMiniGame ();
		break;
	case 3:
		_cameraMovement->MoveTo (_drillPosition, _drillRotation);
		FadeIn (0.75f, 0.5f);
		FadeOut (3.5f, 0.0f);
		break;
	}

	//Disable resources UI
	_uiController->Disable (_uiController->resourcesRef);
	//Disable currentTurn UI
	_uiController->Disable (_uiController->currentTurnRef);
}

void ASimulationGameController::ExitMiniGame ()
{
	GetWorld ()->GetFirstPlayerController ()->Possess (this);

	_miniGameIsActive = false;

	_cameraMovement->MoveTo (_defaultPosition, _defaultRotation);

	//After exiting minigame, start a new turn
	StartNewTurn ();

	//if (_miniGameActive == 1)
	//	_mine->SetActorLocation (FVector (-19420, 1268, -2030));

	_miniGameActive = 0;

	//Enable resources UI
	_uiController->Enable (_uiController->resourcesRef, 0);
	//Enable currentTurn UI
	_uiController->Enable (_uiController->currentTurnRef, 0);
}

void ASimulationGameController::StartNewTurn ()
{
	_currentTurn++;
	currentTurnText = "Turn " + FString::FromInt (_currentTurn);

	//Enable simulation UI
	_uiController->Enable (_uiController->simulationRef, 0);

	_simulation->OnNewTurn (_currentTurn);

	_placing = true;
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

void ASimulationGameController::UpdateMovingToMine ()
{
	if (FVector::Distance (GetActorLocation (), _mineCameraPositions [_currentPositionIndex]) < 400)
	{
		if (_currentPositionIndex == _mineCameraPositions.Num () - 1)
		{
			_cameraMovement->MoveTo (_minePosition, _mineRotation);
			_movingToMine = false;
		}
		else
		{
			_currentPositionIndex++;
			_cameraMovement->MoveTo (_mineCameraPositions [_currentPositionIndex], _mineCameraRotations [_currentPositionIndex]);
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
	//If game hasn't started, start it
	if (!gameStarted)
	{
		gameStarted = true;

		//Enable simulation UI
		_uiController->Enable (_uiController->simulationRef, 0);

		//Enable resources UI
		_uiController->Enable (_uiController->resourcesRef, 0);

		//Enable currentTurn UI
		_uiController->Enable (_uiController->currentTurnRef, 1);

		//Disable menu UI
		_uiController->Disable (_uiController->menuRef);

		//Go to "placing state"
		_placing = true;
		return;
	}

	//Buttons
	/*if (_placing)
	{
		FVector2D mousePosition;
		GetWorld ()->GetFirstPlayerController ()->GetMousePosition (mousePosition.X, mousePosition.Y);

		FVector2D buttonHalfSize = {75, 75};

		FVector2D buttonOnePosition = {660, 900};
		FVector2D buttonTwoPosition = {885, 900};
		FVector2D buttonThreePosition = {1110, 900};

		if (mousePosition.X > buttonOnePosition.X - buttonHalfSize.X && mousePosition.X < buttonOnePosition.X + buttonHalfSize.X &&
			mousePosition.Y > buttonOnePosition.Y - buttonHalfSize.Y && mousePosition.Y < buttonOnePosition.Y + buttonHalfSize.Y)
			SpawnUnit (0);
		else if (mousePosition.X > buttonTwoPosition.X - buttonHalfSize.X && mousePosition.X < buttonTwoPosition.X + buttonHalfSize.X &&
			mousePosition.Y > buttonTwoPosition.Y - buttonHalfSize.Y && mousePosition.Y < buttonTwoPosition.Y + buttonHalfSize.Y)
			SpawnUnit (1);
		else if (mousePosition.X > buttonThreePosition.X - buttonHalfSize.X && mousePosition.X < buttonThreePosition.X + buttonHalfSize.X &&
			mousePosition.Y > buttonThreePosition.Y - buttonHalfSize.Y && mousePosition.Y < buttonThreePosition.Y + buttonHalfSize.Y)
			SpawnUnit (2);
	}*/
}

void ASimulationGameController::OnMouseRelease ()
{
	//If the player currently controls a unit, place it
	if (_placing && _controlledUnit)
		PlaceUnit ();
}

//Called to bind functionality to input
void ASimulationGameController::SetupPlayerInputComponent (UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent (PlayerInputComponent);

	PlayerInputComponent->BindAction ("MouseClick", IE_Pressed, this, &ASimulationGameController::OnMouseClick);
	PlayerInputComponent->BindAction ("MouseClick", IE_Released, this, &ASimulationGameController::OnMouseRelease);
	PlayerInputComponent->BindAction ("Space", IE_Pressed, this, &ASimulationGameController::OnSpacePress);
}
