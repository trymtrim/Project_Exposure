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

	//Will only run while game is running
	UpdateGameState (DeltaTime);
	UpdateObstacles (DeltaTime);
	MovePlane (DeltaTime);
}

void AMinigameDrillController::SetGameController (ASimulationGameController* gameController)
{
	_gameController = gameController;
}

void AMinigameDrillController::StartGame ()
{
	SetActorTickEnabled (true);

	//Enable UI
	showDrillUI = true;

	_gameTimer = 0.0f;
	_health = 3;
	_currentDrillType = 1;
	
	//Spawn drill
	FVector pos = spawnPosition + FVector (0.0f, 0.0f, 450.0f);
	_drill = GetWorld ()->SpawnActor <AActor> (_drillPrefabs [0], pos, rotator, spawnParams);

	FRotator planeRotator = FRotator::MakeFromEuler (FVector (0.0f, 0.0f, 270.0f));
	FVector firstPlaneSpawnPosition = spawnPosition + FVector (0.0f, 0.0f, _height + 500.0f);

	//Spawn planes
	_planeOne = GetWorld ()->SpawnActor <AActor> (_planePrefab, firstPlaneSpawnPosition, planeRotator, spawnParams);

	FVector secondPlaneSpawnPosition = _planeOne->GetActorLocation () - FVector (0.0f, 0.0f, 5000.0f);

	_planeTwo = GetWorld ()->SpawnActor <AActor> (_planePrefab, secondPlaneSpawnPosition, planeRotator, spawnParams);

	//Enable drillMiniGame UI
	_uiController->Enable (_uiController->drillMiniGameRef, 0);
}

void AMinigameDrillController::EndGame ()
{
	SetActorTickEnabled (false);

	//Disable UI
	showDrillUI = false;

	_drill->Destroy ();
	_planeOne->Destroy ();
	_planeTwo->Destroy ();

	//Disable drillMiniGame UI
	_uiController->Disable (_uiController->drillMiniGameRef);

	_gameController->ExitMiniGame ();
}

void AMinigameDrillController::UpdateGameState (float deltaTime)
{
	_gameTimer += deltaTime;

	if (_gameTimer >= 30.0f)
		EndGame ();
}

void AMinigameDrillController::UpdateObstacles (float deltaTime)
{
	_timer += deltaTime;

	if (_timer >= _spawnInterval)
	{
		SpawnObstacle ();
		_timer = 0.0f;
	}
}

void AMinigameDrillController::SpawnObstacle ()
{
	FActorSpawnParameters obstacleSpawnParams;
		
	FVector obstacleSpawnPosition = FVector (_drill->GetActorLocation ().X, _drill->GetActorLocation ().Y, _drill->GetActorLocation ().Z - 2000.0f);
	FRotator obstacleRotator = FVector (0.0f, 0.0f, 0.0f).Rotation ();

	//Spawn random obstacle from array of obstacles
	TSubclassOf <AActor> obstacle = _obstaclePrefabs [FMath::RandRange (0, _obstaclePrefabs.Num () - 1)];
	ADrillObstacle* spawnedObstacle = GetWorld ()->SpawnActor <ADrillObstacle> (obstacle, obstacleSpawnPosition, obstacleRotator, obstacleSpawnParams);

	spawnedObstacle->Initialize (_drill, this);

	//Set time before next spawn
	_spawnInterval = FMath::RandRange (5, 25) / 10.0f;
}

void AMinigameDrillController::ChangeDrill (int index)
{
	_currentDrillType = index;

	//Remove current drill
	_drill->Destroy ();

	//Spawn new drill
	FVector pos = spawnPosition + FVector (0.0f, 0.0f, 450.0f);
	_drill = GetWorld ()->SpawnActor <AActor> (_drillPrefabs [index - 1], pos, rotator, spawnParams);
}

void AMinigameDrillController::MovePlane (float deltaTime)
{
	_planeOne->SetActorLocation (_planeOne->GetActorLocation () + FVector (0.0f, 0.0f, 600.0f) * deltaTime);
	_planeTwo->SetActorLocation (_planeTwo->GetActorLocation () + FVector (0.0f, 0.0f, 600.0f) * deltaTime);

	if (_planeOne->GetActorLocation ().Z > 2000.0f)
		_planeOne->SetActorLocation (_planeTwo->GetActorLocation () - FVector (0.0f, 0.0f, 5000.0f));
	else if (_planeTwo->GetActorLocation ().Z > 2000.0f)
		_planeTwo->SetActorLocation (_planeOne->GetActorLocation () - FVector (0.0f, 0.0f, 5000.0f));
}

void AMinigameDrillController::GetHitByObstacle ()
{
	_health--;

	if (_health == 0)
		EndGame ();
}

int AMinigameDrillController::GetCurrentDrillType ()
{
	return _currentDrillType;
}

//Called to bind functionality to input
void AMinigameDrillController::SetupPlayerInputComponent (UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent (PlayerInputComponent);
}
