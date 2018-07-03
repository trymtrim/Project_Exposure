//Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameDrillController.h"
#include "SimulationGameController.h"
#include "DrillObstacle.h"
#include "UIController.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

//Sets default values
AMinigameDrillController::AMinigameDrillController ()
{
 	//Set this pawn to call Tick () every frame.
	PrimaryActorTick.bCanEverTick = true;
}

//Called when the game starts or when spawned
void AMinigameDrillController::BeginPlay ()
{
	Super::BeginPlay ();

	SetActorTickEnabled (false);

	spawnPosition = FVector (3650.0f, -3050.0f, _height + 420.0f);
	rotator = FRotator::MakeFromEuler (FVector (0.0f, 0.0f, 0.0f));
}

//Called every frame
void AMinigameDrillController::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);

	if (_animatingStars)
		AnimateStars (DeltaTime);
	else
	{
		//Will only run while game is running
		if (_gameStarted)
		{
			UpdateGameState (DeltaTime);
			UpdateObstacles (DeltaTime);
			MovePlane (DeltaTime);
		}

		if (_disableUI)
		{
			_disableUITimer += DeltaTime;

			if (_disableUITimer >= 0.8f)
			{
				_disableUITimer = 0.0f;
				_disableUI = false;

				StartGamePlay ();
			}
		}
	}
}

void AMinigameDrillController::SetGameController (ASimulationGameController* gameController)
{
	_gameController = gameController;
}

void AMinigameDrillController::StartGame ()
{
	SetActorTickEnabled (true);

	SetLives (3);
	SetScore (0);

	_endTimer = 0.0f;
	_currentDrillType = 1;
	_spawnCount = 0;

	_gameFinished = false;
	_gameCompleted = false;
	
	//Spawn drill
	FVector pos = spawnPosition + FVector (0.0f, 0.0f, 492.0f);
	_drill = GetWorld ()->SpawnActor <AActor> (_drillPrefabs [0], pos, rotator, spawnParams);

	//Spawn hole
	FVector holePos = spawnPosition + FVector (0.0f, 0.0f, 542.0f);
	_hole = GetWorld ()->SpawnActor <AActor> (_holePrefab, holePos, rotator, spawnParams);

	FRotator planeRotator = FRotator::MakeFromEuler (FVector (0.0f, 0.0f, 270.0f));
	FVector firstPlaneSpawnPosition = spawnPosition + FVector (0.0f, 50.0f, _height + 500.0f); //Y is Z

	//Spawn planes
	_planeOne = GetWorld ()->SpawnActor <AActor> (_planePrefab, firstPlaneSpawnPosition, planeRotator, spawnParams);

	FVector secondPlaneSpawnPosition = _planeOne->GetActorLocation () - FVector (0.0f, 0.0f, 5000.0f);

	_planeTwo = GetWorld ()->SpawnActor <AActor> (_planePrefab, secondPlaneSpawnPosition, planeRotator, spawnParams);

	//Enable animation2 UI
	_uiController->Enable (13, 0);

	//For blueprint
	Start ();

	_endPanelShown = false;
}

void AMinigameDrillController::StartGamePlay ()
{
	_gameStarted = true;

	//Enable drillMiniGame UI
	_uiController->Enable (4, 0);
}

void AMinigameDrillController::StartDisableUI ()
{
	_disableUI = true;

	//Disable animation2 UI
	_uiController->Disable (13);

	//Enable animation UI
	_uiController->Enable (9, 2);
}

void AMinigameDrillController::EndGame ()
{
	if (_score < 30 / 3)
	{
		starAmount = 1;
		if (_gameController->GetCurrentTurn() == 1) _gameController->SetMinigamePerformance(_gameController->NORMAL);
		else _gameController->SetMinigamePerformance(_gameController->BAD);
	}
	else if (_score > 30 / 3 && _score < 30 - (30 /3))
	{
		starAmount = 2;
		_gameController->SetMinigamePerformance (_gameController->NORMAL);
	}
	else 
	{
		starAmount = 3;
		_gameController->SetMinigamePerformance (_gameController->GOOD);
	}

	if (_lives == 0)
	{
		//Enable lose panel UI
		_uiController->Enable (8, 2);
	}
	else
	{
		//Enable win panel UI
		_uiController->Enable (8, 2);
	}

	//Disable drillMiniGame UI
	_uiController->Disable (4);

	_animatingStars = true;

	_endPanelShown = true;
	_gameStarted = false;
	_gameController->StartClickDelay ();
}

void AMinigameDrillController::AnimateStars (float deltaTime)
{
	_starLerp += deltaTime;

	if (_starLerp > 0.075f)
	{
		_starLerp = 0.0f;
		_currentScoreLerp++;
		_barValue = (float) _currentScoreLerp / 30.0f;

		endScoreText = FString::FromInt (_currentScoreLerp) + "/30";
		EndScreen ();

		if (_currentScoreLerp == 1 || _score == 0)
			OneStar ();
		else if (_currentScoreLerp == 11)
			TwoStar ();
		else if (_currentScoreLerp == 21)
			ThreeStars ();

		if (_currentScoreLerp == _score || _score == 0)
		{
			_animatingStars = false;
			_starLerp = 0.0f;
			_currentScoreLerp = 0;
			_barValue = 0.0f;
	
			SetActorTickEnabled (false);
		}
	}
}

void AMinigameDrillController::GoBackToSimulation ()
{
	_drill->Destroy ();
	_hole->Destroy ();
	_planeOne->Destroy ();
	_planeTwo->Destroy ();

	if (_oilPlane)
	{
		_oilPlane->Destroy ();
		_oilPlane = nullptr;
	}

	_gameCompleted = true;

	Stop ();

	if (_lives <= 0)
	{
		//Disable lose panel UI
		_uiController->Disable (8);
	}
	else
	{
		//Disable win panel UI
		_uiController->Disable (8);
	}

	//Disable animation UI
	_uiController->Disable (9);

	_gameController->ExitMiniGame ();
}

void AMinigameDrillController::UpdateGameState (float deltaTime)
{
	if (_gameFinished)
	{
		_endTimer += deltaTime;

		if (_endTimer >= 5.5f)
			EndGame ();
	}
}

void AMinigameDrillController::UpdateObstacles (float deltaTime)
{
	_timer += deltaTime;

	if (!_gameFinished && _timer >= _spawnInterval)
	{
		SpawnObstacle ();
		_timer = 0.0f;
	}
}

void AMinigameDrillController::SpawnObstacle ()
{
	FActorSpawnParameters obstacleSpawnParams;
		
	FVector obstacleSpawnPosition = FVector (_drill->GetActorLocation ().X, _drill->GetActorLocation ().Y + 50.0f, _drill->GetActorLocation ().Z - 2000.0f);
	FRotator obstacleRotator = FVector (0.0f, 0.0f, 0.0f).Rotation ();

	//Spawn random obstacle from array of obstacles
	TSubclassOf <AActor> obstacle = _obstaclePrefabs [FMath::RandRange (0, _obstaclePrefabs.Num () - 1)];
	ADrillObstacle* spawnedObstacle = GetWorld ()->SpawnActor <ADrillObstacle> (obstacle, obstacleSpawnPosition, obstacleRotator, obstacleSpawnParams);

	spawnedObstacle->Initialize (_drill, this);

	//Set time before next spawn
	_spawnInterval = FMath::RandRange (15, 25) / 10.0f;
	_spawnInterval = 2.0f;
	_spawnInterval -= _spawnCount * 0.025f;
	//_spawnInterval = FMath::RandRange ((5, 25) / 10.0f) / (_spawnCount / 15.0f);
	_spawnCount++;

	if (_spawnCount == 30)
	{
		_gameFinished = true;

		//Spawn oil plane
		FActorSpawnParameters oilSpawnParams;
		FVector oilSpawnPosition = _drill->GetActorLocation () - FVector (0.0f, 0.0f, 6500.0f);
		FRotator oilRotator = FRotator (0.0f, 0.0f, 90.0f);
		
		_oilPlane = GetWorld ()->SpawnActor <AActor> (_oilPlanePrefab, oilSpawnPosition, oilRotator, oilSpawnParams);
	}
}

void AMinigameDrillController::ChangeDrill (int index)
{
	_currentDrillType = index;

	//Remove current drill
	_drill->Destroy ();

	//Spawn new drill
	FVector pos = spawnPosition + FVector (0.0f, 0.0f, 492.0f);
	_drill = GetWorld ()->SpawnActor <AActor> (_drillPrefabs [index - 1], pos, rotator, spawnParams);

	if (_gameController->GetInOptions ())
		_gameController->GoBackToGame ();
}

void AMinigameDrillController::MovePlane (float deltaTime)
{
	float speed = 800.0f;
	speed += _spawnCount * 17.5f;

	_planeOne->SetActorLocation (_planeOne->GetActorLocation () + FVector (0.0f, 0.0f, speed) * deltaTime);
	_planeTwo->SetActorLocation (_planeTwo->GetActorLocation () + FVector (0.0f, 0.0f, speed) * deltaTime);

	if (_planeOne->GetActorLocation ().Z > 6000.0f)
		_planeOne->SetActorLocation (_planeTwo->GetActorLocation () - FVector (0.0f, 0.0f, 5000.0f));
	else if (_planeTwo->GetActorLocation ().Z > 6000.0f)
		_planeTwo->SetActorLocation (_planeOne->GetActorLocation () - FVector (0.0f, 0.0f, 5000.0f));

	if (_oilPlane && _endTimer < 4.0f)
		_oilPlane->SetActorLocation (_oilPlane->GetActorLocation () + FVector (0.0f, 0.0f, speed) * deltaTime);
}

int AMinigameDrillController::GetSpawnCount ()
{
	return _spawnCount;
}

void AMinigameDrillController::SetLives (int lives)
{
	_lives = lives;
	livesText = FString::FromInt (_lives);

	Update ();
}

void AMinigameDrillController::SetScore (int score)
{
	_score = score;

	scoreText = FString::FromInt (_score);

	Update ();
}

void AMinigameDrillController::GetHitByObstacle ()
{
	SetLives (_lives - 1);

	//Camera shake
	GetWorld ()->GetFirstPlayerController ()->ClientPlayCameraShake (_cameraShake);

	if (_lives == 0)
		EndGame ();
}

void AMinigameDrillController::OvercomeObstacle ()
{
	SetScore (_score + 1);
}

bool AMinigameDrillController::GetGameFinished ()
{
	if (_lives <= 0)
		return true;
	
	return false;
}

bool AMinigameDrillController::GetGameCompleted ()
{
	return _gameCompleted;
}

int AMinigameDrillController::GetCurrentDrillType ()
{
	return _currentDrillType;
}

void AMinigameDrillController::OnMouseClick ()
{
	if (_endPanelShown)
	{
		if (_gameController->CanContinue ())
			GoBackToSimulation ();
	}
	else if (!_gameStarted && !_disableUI)
		StartDisableUI ();

	if (_gameController->GetInOptions ())
		_gameController->GoBackToGame ();
}

//Called to bind functionality to input
void AMinigameDrillController::SetupPlayerInputComponent (UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent (PlayerInputComponent);

	PlayerInputComponent->BindAction ("MouseClick", IE_Pressed, this, &AMinigameDrillController::OnMouseClick);
}
