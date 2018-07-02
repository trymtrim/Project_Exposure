//Fill out your copyright notice in the Description page of Project Settings.

#include "SimulationGameController.h"
#include "Engine/World.h"
#include "CustomGameViewportClient.h"
#include "Runtime/UMG/Public/UMG.h"
#include "UserWidget.h"
#include "Slate.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include "MainGameInstance.h"

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

	//Debug
	if (!_miniGamesOn)
	{
		_mineGamePlayed = true;
		_oilGamePlayed = true;
		_windGamePlayed = true;
	}

	//Setup camera
	_defaultPosition = GetActorLocation ();
	_defaultRotation = FVector (0.0f, 324.0f, 135.0f);

	_cameraMovement = new CameraMovement (this);

	//Setup pawns
	_drillPawn->SetGameController (this);
	_windmillPawn->SetGameController (this);

	FadeOut (0.5f, 1.5f);

	InitializeStartUI ();

	//Initialize highscores
	_overallHighscore = new Highscore (FPaths::ProjectContentDir () + "../highscores/Highscore.csv");

	//Get current date and time for daily highscore filepath
	auto t = std::time (nullptr);
	auto tm = *std::localtime (&t);

	std::ostringstream dateStream;
	dateStream << std::put_time (&tm, "%d.%m.%Y");

	std::string date = dateStream.str ();

	FString dailyFilePath = date.c_str ();

	_dailyHighscore = new Highscore (FPaths::ProjectContentDir () + "../highscores/daily/" + dailyFilePath + ".csv");

	//_overallHighscore->AddScore ("yo", 150);
	//_dailyHighscore->AddScore ("yo", 150);
}

//Called every frame
void ASimulationGameController::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);

	if (_gameFinished)
		return;

	//Disable the black start panel after certain amount of seconds
	if (_startPanelEnabled)
	{
		_startPanelTimer += DeltaTime;

		if (_startPanelTimer >= 0.5f)
		{
			_uiController->Disable (16);
			_startPanelEnabled = false;
		}
	}

	if (!_uiEnabled)
	{
		//Enable resources UI
		//_uiController->Enable (2, 0);

		//Enable currentTurn UI
		_uiController->Enable (0, 1);

		//Enable reset button UI
		_uiController->Enable (5, 3);

		//Disable menu UI
		_uiController->Disable (3);

		//Go to "placing state"
		_placing = true;

		_uiEnabled = true;

		GoToNextUI ();
	}

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
			GetWorld ()->GetFirstPlayerController ()->Possess (_windmillPawn);
			_windmillPawn->StartGame ();
			break;
		case 3:
			GetWorld ()->GetFirstPlayerController ()->Possess (_drillPawn);
			_drillPawn->StartGame ();
			break;
		}

		_miniGameIsActive = true;
	}
	else if (!_waitingForResponse && _playSimulation)
	{
		_playSimulation = false;
		StartSimulation ();
	}

	if (fadingIn || fadingOut)
		UpdateFading (DeltaTime);

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

	if (_panelDelay)
	{
		_panelTimer += DeltaTime;

		if (_panelTimer >= _panelDelayTime)
		{
			_panelTimer = 0.0f;
			_panelDelay = false;
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
		_uiController->Disable (1);
		//Enable thrash UI
		_uiController->Enable (18, 0);

		ClearRemovable ();
	}
}

void ASimulationGameController::PlaceUnit ()
{
	if (!_controlledUnit)
		return;

	_replayPP = nullptr;

	//If the video-buttuon is pressed
	APlayerController* playerController = GetWorld ()->GetFirstPlayerController ();
	FVector2D mousePos;
	playerController->GetMousePosition (mousePos.X, mousePos.Y);

	FIntVector viewportBounds;
	playerController->GetViewportSize (viewportBounds.X, viewportBounds.Y);

	FVector2D percentage = FVector2D (mousePos.X / viewportBounds.X * 100, mousePos.Y / viewportBounds.Y * 100);

	if (percentage.X >= 44 && percentage.X <= 55 && percentage.Y >= 81)
	{
		//Enable simulation UI
		_uiController->Enable (1, 0);

		//Remove currently controlled unit
		_controlledUnit->Destroy ();
		_controlledUnit = nullptr;
	}
	else if (_controlledUnit->PlaceUnit ())
	{
		_powerPlants.Add (_controlledUnit);

		int miniGameType = _controlledUnit->GetTypeIndex ();

		_miniGameActive = miniGameType;

		//Spawn the optional minigame message
		FActorSpawnParameters spawnParams;
		FVector spawnPosition = _controlledUnit->GetActorLocation () + FVector (0, 0, 500);

		FVector rotation = spawnPosition - GetActorLocation ();
		FRotator rotator = rotation.Rotation ();

		//If the current mini game has been played before, give the player the option to play it again
		if (_miniGameActive == 1 && _mineGamePlayed || _miniGameActive == 2 && _windGamePlayed || _miniGameActive == 3 && _oilGamePlayed)
		{
			_messageBox = GetWorld ()->SpawnActor <AActor> (_optionalMinigameMessage, spawnPosition + FVector (0, 0, 500), rotator, spawnParams);
			_replayPP = _controlledUnit;
		}

		_controlledUnit = nullptr;

		//Check if player wants to play minigame first
		if (_miniGameActive == 1 && !_mineGamePlayed || _miniGameActive == 2 && !_windGamePlayed || _miniGameActive == 3 && !_oilGamePlayed)
			EnterMiniGame ();
		else
		{
			_playSimulation = true;
			_waitingForResponse = true;
		}

		_placing = false;
		ActivateOutlines (false);
	}
	else
	{
		//Enable simulation UI
		_uiController->Enable (1, 0);

		//Remove currently controlled unit
		_controlledUnit->Destroy ();
		_controlledUnit = nullptr;
	}

	//Disable thrash UI
	_uiController->Disable (18);
}

void ASimulationGameController::RemoveUnit ()
{
	if (_controlledUnit)
		return;

	//Trace to see what is under the mouse cursor
	FHitResult hit;
	GetWorld ()->GetFirstPlayerController ()->GetHitResultUnderCursor (ECC_Visibility, true, hit);

	if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Nuclear") || hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Windmill") || hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Oil") || hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("message"))
	{
		APlaceableUnit* unit = nullptr;

		if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("message"))
			unit = Cast <APlaceableUnit> (_removePP);
		else
			unit = Cast <APlaceableUnit> (hit.GetActor ());
	
		if (unit->GetRemovable ())
		{
			if (hit.bBlockingHit)
			{
				switch (unit->GetTypeIndex ())
				{
				case 1:
					_simulation->OnRemoveUnit (unit);
					break;
				case 2:
					_simulation->OnRemoveUnit (unit);
					break;
				case 3:
					_simulation->OnRemoveUnit (unit);
					break;
				}

				unit->Destroy ();
				_removeBox->Destroy ();

				_powerPlants.Remove (unit);
				_removePP = nullptr;
			}
		}
		else
		{
			if (_removeBox)
				_removeBox->Destroy ();
			if (_removePP)
				Cast <APlaceableUnit> (_removePP)->SetRemovable (false);

			unit->SetRemovable (true);
			_removePP = unit;
			
			//Spawn the remove message
			FActorSpawnParameters spawnParams;
			FVector spawnPosition = unit->GetActorLocation () + FVector (0, 0, 750);

			FVector rotation = spawnPosition - GetActorLocation ();
			FRotator rotator = rotation.Rotation ();

			_removeBox = GetWorld ()->SpawnActor <AActor> (_removeMessage, spawnPosition, rotator, spawnParams);
		}
	}
	else
		ClearRemovable ();
}

void ASimulationGameController::ClearRemovable ()
{
	if (_removeBox)
	{
		for (int i = 0; i < _powerPlants.Num (); i++)
		{
			if (_powerPlants [i]->GetRemovable ())
				_powerPlants [i]->SetRemovable (false);
		}

		_removeBox->Destroy ();
		_removePP = nullptr;
	}
}

void ASimulationGameController::ActivateOutlines (bool status)
{
	for (int i = 0; i < _powerPlants.Num (); i++)
		_powerPlants [i]->ActivateOutline (status);
}

void ASimulationGameController::SetMinigamePerformance (MinigamePerformance performance)
{
	switch (performance)
	{
	case BAD:
		_minigamePerformance = 0.75f;
		break;
	case NORMAL:
		_minigamePerformance = 1.0f;
		break;
	case GOOD:
		_minigamePerformance = 1.25f;
		break;
	}
}

void ASimulationGameController::ShowPermanentPollutionMessage ()
{
	_showPermanentPollutionMessage = true;
}

void ASimulationGameController::StartSimulation ()
{
	_simulationRunning = true;

	_powerPlants [_powerPlants.Num () -1]->SetPerformancePercentage (_minigamePerformance);
	_simulation->OnPlaceUnit (_powerPlants [_powerPlants.Num () - 1]);

	_miniGameActive = 0;
	_minigamePerformance = 1.0f;

	hourGlassRef->AddToViewport (0);

	_simulation->StartSimulation ();
}

void ASimulationGameController::StopSimulation ()
{
	StartNewTurn ();

	if (_messageBox)
		_messageBox->Destroy ();

	_simulationRunning = false;

	hourGlassRef->RemoveFromParent ();

	_simulation->StopSimulation ();
}

void ASimulationGameController::EnterMiniGame ()
{
	FVector rot;

	switch (_miniGameActive)
	{
	case 1:
		_cameraMovement->MoveTo (_mineCameraPositions [0], _mineCameraRotations [0]);
		_movingToMine = true;
		_mineGamePlayed = true;
		break;
	case 2:
		rot = GetActorLocation () - _powerPlants [_powerPlants.Num () - 1]->GetActorLocation ();
		_cameraMovement->MoveTo (_powerPlants [_powerPlants.Num () - 1]->GetActorLocation (), rot.Rotation ().Vector ());
		_windGamePlayed = true;
		FadeIn (0.5f, 0.5f);
		FadeOut (3.5f, 0.5f);
		break;
	case 3:
		_cameraMovement->MoveTo (_drillPosition, _drillRotation);
		_oilGamePlayed = true;
		FadeIn (0.75f, 0.5f);
		FadeOut (3.5f, 0.5f);
		break;
	}

	//Disable resources UI
	//_uiController->Disable (2);
	//Disable currentTurn UI
	_uiController->Disable (0);
}

void ASimulationGameController::ExitMiniGame ()
{
	GetWorld ()->GetFirstPlayerController ()->Possess (this);

	_miniGameIsActive = false;

	//After exiting minigame, play simulation
	_playSimulation = true;

	if (_miniGameActive == 1)
	{
		_cameraMovement->MoveTo (_mineCameraPositions [_mineCameraPositions.Num () - 1], _mineCameraRotations [_mineCameraRotations.Num () - 1]);
		_movingFromMine = true;
	}
	else
		_cameraMovement->MoveTo (_defaultPosition, _defaultRotation);

	//Enable resources UI
	//_uiController->Enable (2, 0);
	//Enable currentTurn UI
	_uiController->Enable (0, 0);
}

void ASimulationGameController::StartClickDelay ()
{
	_panelDelay = true;
}

void ASimulationGameController::EndGame (bool gameWon)
{
	if (gameWon)
	{
		//Enable win UI
		_uiController->Enable (22, 0);
	}
	else
	{
		//Enable lose UI
		_uiController->Enable (22, 0);
	}

	_gameFinished = true;
}

bool ASimulationGameController::CanContinue ()
{
	return !_panelDelay;
}

TArray <int> ASimulationGameController::GetHighscores (bool daily)
{
	if (daily)
		return _dailyHighscore->GetHighscores ();

	return _overallHighscore->GetHighscores ();
}

TArray <FString> ASimulationGameController::GetHighscoreNames (bool daily)
{
	if (daily)
		return _dailyHighscore->GetHighscoreNames ();

	return _overallHighscore->GetHighscoreNames ();
}

FString ASimulationGameController::GetPlayerName ()
{
	return Cast <UMainGameInstance> (GetWorld ()->GetGameInstance ())->PlayerName;
}

void ASimulationGameController::StartNewTurn ()
{
	_currentTurn++;
	currentTurnText = "Jaar " + FString::FromInt (_currentTurn);

	_simulation->OnNewTurn (_currentTurn);

	_placing = true;
	ActivateOutlines (true);

	if (_showPermanentPollutionMessage)
	{
		permanentPollutionMessageRef->AddToViewport (3);

		_showingPollutionMessage = true;
		_showPermanentPollutionMessage = false;
	}
	else
	{
		//Enable simulation UI
		_uiController->Enable (1, 0);
	}
}

void ASimulationGameController::StopShowingPollutionMessage (int index)
{
	if (index == 1)
	{
		permanentPollutionMessageRef->RemoveFromParent ();
		permanentPollutionMessage2Ref->AddToViewport (3);

		_showingPollutionMessage = false;
		_showingPollutionMessage2 = true;
	}
	else
	{
		permanentPollutionMessage2Ref->RemoveFromParent ();

		//Enable simulation UI
		_uiController->Enable (1, 0);

		_showingPollutionMessage2 = false;
	}
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

void ASimulationGameController::InitializeStartUI ()
{
	for (int i = 0; i < UIs.Num (); i++)
		uiRefs.Add (CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), UIs [i]));

	permanentPollutionMessageRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), permanentPollutionMessage);
	permanentPollutionMessage2Ref = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), permanentPollutionMessage2);
	hourGlassRef = CreateWidget <UUserWidget> (GetWorld ()->GetFirstPlayerController (), hourGlass);
}

void ASimulationGameController::GoToNextUI ()
{
	if (_currentUI == uiRefs.Num ())
	{
		uiRefs [_currentUI - 1]->RemoveFromParent ();

		//Enable simulation UI
		_uiController->Enable (1, 0);

		_showingUI = false;
	}
	else
	{
		if (_currentUI == 0)
			_showingUI = true;
		else
			uiRefs [_currentUI - 1]->RemoveFromParent ();

		uiRefs [_currentUI]->AddToViewport (3);

		_currentUI++;
	}
}

void ASimulationGameController::CheckAFK ()
{

}

void ASimulationGameController::QuitGame ()
{
	delete _cameraMovement;
	delete _overallHighscore;
	delete _dailyHighscore;

	ReloadGame ();
}

void ASimulationGameController::OnSpacePress ()
{
	if (_simulationRunning)
		StopSimulation ();
}

int ASimulationGameController::GetCurrentTurn ()
{
	return _currentTurn;
}

void ASimulationGameController::OnMouseClick ()
{
	if (_gameFinished)
		ReloadGame ();
	else if (_showingPollutionMessage)
		StopShowingPollutionMessage (1);
	else if (_showingPollutionMessage2)
		StopShowingPollutionMessage (2);
	else if (_waitingForResponse)
	{
		//Trace to see what is under the mouse cursor
		FHitResult hit;
		GetWorld ()->GetFirstPlayerController ()->GetHitResultUnderCursor (ECC_Visibility, true, hit);

		if (hit.bBlockingHit)
		{
			if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("message"))
			{
				hit.GetActor ()->Destroy ();
				_playSimulation = false;
				EnterMiniGame ();
			}
			else if (hit.GetActor () == _replayPP)
			{
				_messageBox->Destroy ();
				_playSimulation = false;
				EnterMiniGame ();
			}
		}

		if (_messageBox)
			_messageBox->Destroy ();

		_waitingForResponse = false;
	}
	else if (_showingUI)
		GoToNextUI ();
	else if (_placing)
		RemoveUnit ();
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
