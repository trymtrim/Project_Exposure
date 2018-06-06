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

	//Enable simulationTest UI
	_uiController->Enable (_uiController->simulationTestRef, 0);
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
		UpdateMovingToMine (true);
	else if (_movingFromMine)
		UpdateMovingToMine (false);

	if (_simulationRunning)
	{
		_simulationTimer += DeltaTime;

		if (_simulationTimer > _simulationTime)
		{
			_simulationTimer = 0.0f;
			StopSimulation ();
		}
	}
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

		//Spawn the optional minigame message
		FActorSpawnParameters spawnParams;
		FVector spawnPosition = _controlledUnit->GetActorLocation () + FVector (0, 0, 500);
		FRotator rotator = FVector (0.0f, 0.0f, 0.0f).Rotation ();

		//If the current mini game has been played before, give the player the option to play it again
		if (_miniGameActive == 1 && _mineGamePlayed || _miniGameActive == 2 && _windGamePlayed || _miniGameActive == 3 && _oilGamePlayed)
			_messageBox = GetWorld ()->SpawnActor <AActor> (_optionalMinigameMessage, spawnPosition + FVector (0, 0, 500), rotator, spawnParams);

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
	//_uiController->Enable (_uiController->simulationTestRef, 0);

	_simulationRunning = true;
}

void ASimulationGameController::StopSimulation ()
{
	//Disable simulationTest UI
	//_uiController->Disable (_uiController->simulationTestRef);

	if (_miniGameActive == 1 && !_mineGamePlayed || _miniGameActive == 2 && !_windGamePlayed || _miniGameActive == 3 && !_oilGamePlayed)
	{
		if (_miniGamesOn)
			EnterMiniGame ();
	}
	else
		StartNewTurn ();

	if (_messageBox)
		_messageBox->Destroy ();

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
		_mineGamePlayed = true;
		break;
	case 2:
		ExitMiniGame ();

		_windGamePlayed = true;
		break;
	case 3:
		_cameraMovement->MoveTo (_drillPosition, _drillRotation);
		FadeIn (0.75f, 0.5f);
		FadeOut (3.5f, 0.0f);
		_oilGamePlayed = true;
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

	//After exiting minigame, start a new turn
	StartNewTurn ();

	if (_miniGameActive == 1)
	{
		_cameraMovement->MoveTo (_mineCameraPositions [_mineCameraPositions.Num () - 1], _mineCameraRotations [_mineCameraRotations.Num () - 1]);
		_movingFromMine = true;
	}
	else
		_cameraMovement->MoveTo (_defaultPosition, _defaultRotation);

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

void ASimulationGameController::UpdateMovingToMine (bool toMine)
{
	if (toMine)
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
	else
	{
		if (FVector::Distance (GetActorLocation (), _mineCameraPositions [_currentPositionIndex]) < 400)
		{
			if (_currentPositionIndex == 0)
			{
				_cameraMovement->MoveTo (_defaultPosition, _defaultRotation);
				_movingFromMine = false;
			}
			else
			{
				_currentPositionIndex--;
				_cameraMovement->MoveTo (_mineCameraPositions [_currentPositionIndex], _mineCameraRotations [_currentPositionIndex]);
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

	ResetGame ();
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

	if (_simulationRunning)
	{
		//Trace to see what is under the mouse cursor
		FHitResult hit;
		GetWorld ()->GetFirstPlayerController ()->GetHitResultUnderCursor (ECC_Visibility, true, hit);

		if (hit.bBlockingHit)
		{
			if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("message"))
				hit.GetActor ()->Destroy ();
		}
	}
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
