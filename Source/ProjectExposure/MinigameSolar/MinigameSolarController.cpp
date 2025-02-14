//Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameSolarController.h"
#include "SimulationGameController.h"
#include "Engine/World.h"
#include "UIController.h"
#include "ConstructorHelpers.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

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

	_indexes.Add (0);
	_indexes.Add (1);
	_indexes.Add (2);
	_indexes.Add (3);
}

//Called every frame
void AMinigameSolarController::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);

	if (_animatingStars)
		AnimateStars (DeltaTime);
	else
	{
		if (_controlledUnit)
		{
			//Trace to see what is under the mouse cursor
			FHitResult hit;
			GetWorld ()->GetFirstPlayerController ()->GetHitResultUnderCursor (ECC_Visibility, true, hit);

			if (hit.bBlockingHit)
			{
				FVector position = FVector (hit.Location.X, hit.Location.Y, _middlePosition.Z);
				_controlledUnit->SetActorLocation (position);
			}
		}
	}

	if (_gameStarted)
	{
		_timer -= DeltaTime;

		timerText = "00:" + FString::FromInt ((int) _timer); //REMINDER - FORMAT

		if (_timer <= 0.0f)
			EndGame ();
	}

	if (_pressing)
	{
		_pressingTimer += DeltaTime;

		if (_pressingTimer >= _delayTime)
		{
			ControlUnit ();

			_pressing = false;
			_pressingTimer = 0.0f;
		}
	}
}

void AMinigameSolarController::AnimateStars (float deltaTime)
{
	_starLerp += deltaTime;

	if (_starLerp > 0.075f)
	{
		_starLerp = 0.0f;
		_currentScoreLerp++;
		_barValue = (float) _currentScoreLerp / _maxScore;

		endScoreText = "00:" + FString::FromInt (_currentScoreLerp);
		EndScreen ();

		int points = _currentScoreLerp + 1;

		if (_currentScoreLerp == 1 || _score == 0)
			OneStar ();
		else if (points == _maxScore / 3)
			TwoStar ();
		else if (points == ((( _maxScore / 3) * 2) + 1))
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

void AMinigameSolarController::InitializeGame ()
{
	if (_indexes.Num () == 0)
		_currentIndex = 0;
	else
	{
		int tempIndex = _indexes [FMath::RandRange (0, _indexes.Num () - 1)];
		_currentIndex = tempIndex,
		_indexes.Remove (tempIndex);
	}

	InitializeGrid ();

	UWorld* world = GetWorld ();

	if (world)
	{
		//Spawn mirrors
		FActorSpawnParameters spawnParams;
		FVector spawnPosition = _middlePosition + FVector (175.0f, 275.0f, 0.0f);
		FRotator rotator = FRotator (0.0f, 45.0f, 0.0f);

		for (int i = 0; i < _mirrorCount [_currentIndex]; i++)
		{
			AActor* mirror = world->SpawnActor <AActor> (_mirror, spawnPosition, rotator, spawnParams);
			_mirrors.Add (mirror);
			_mirrorRotations.Add (mirror, 2);

			spawnPosition.X -= 100.0f;
		}

		//Spawn beam start position
		switch (_beamStartRotation [_currentIndex])
		{
		case 1:
			rotator = FRotator (0.0f, 90.0f, 0.0f);
			break;
		case 2:
			rotator = FRotator (0.0f, -180.0f, 0.0f);
			break;
		case 3:
			rotator = FRotator (0.0f, -90.0f, 0.0f);
			break;
		case 4:
			rotator = FRotator (0.0f, 0.0f, 0.0f);
			break;
		}

		spawnPosition = _grid [((_gridX [_currentIndex] * _gridY [_currentIndex]) - 1) / 2]->GetActorLocation () + FVector (_beamStartPositionX [_currentIndex] * 100, _beamStartPositionY [_currentIndex] * 100, 0.0f);
		_beamPosition = world->SpawnActor <AActor> (_gridSlot, spawnPosition, rotator, spawnParams);
		//Spawn beam end position
		spawnPosition = _grid [((_gridX [_currentIndex] * _gridY [_currentIndex]) - 1) / 2]->GetActorLocation () + FVector (_beamGoalPositionX [_currentIndex] * 100, _beamGoalPositionY [_currentIndex] * 100, 0.0f);
		_goalPosition = world->SpawnActor <AActor> (_goal, spawnPosition, rotator, spawnParams);
		//Spawn beam goal target
		spawnPosition += FVector (0.0f, 0.0f, 120.0f);
		_goalTarget = world->SpawnActor <AActor> (_goalTargetPrefab, spawnPosition, rotator, spawnParams);

		//Spawn obstacles
		switch (_currentIndex)
		{
		case 0:
			AddObstacle (2);
			AddObstacle (4);
			AddObstacle (19);
			AddObstacle (13);
			AddObstacle (14);
			break;
		case 1:
			AddObstacle (5);
			AddObstacle (7);
			AddObstacle (15);
			AddObstacle (13);
			AddObstacle (23);
			break;
		case 2:
			AddObstacle (12);
			AddObstacle (11);
			AddObstacle (9);
			AddObstacle (20);
			AddObstacle (1);
			AddObstacle (18);
			AddObstacle (17);
			break;
		case 3:
			AddObstacle (8);
			AddObstacle (1);
			AddObstacle (24);
			AddObstacle (12);
			AddObstacle (16);
			AddObstacle (6);
			break;
		}
	}
}

void AMinigameSolarController::AddObstacle (int gridIndex)
{
	FActorSpawnParameters spawnParams;
	FRotator rotator = FRotator (0.0f, 0.0f, 0.0f);

	FVector spawnPosition = _grid [gridIndex]->GetActorLocation () + FVector (0.0f, 0.0f, 50.0f);

	AActor* obstacle = GetWorld ()->SpawnActor <AActor> (_obstaclePrefab, spawnPosition, rotator, spawnParams);
	_obstacles.Add (obstacle);
}

void AMinigameSolarController::InitializeGrid ()
{
	UWorld* world = GetWorld ();

	int slotSize = 100;

	if (world)
	{
		FActorSpawnParameters spawnParams;
		FVector spawnPosition = _middlePosition - FVector ((slotSize * _gridX [_currentIndex]) / 2, (slotSize * _gridY [_currentIndex]) / 2, 0.0f);
		FRotator rotator = FVector (0.0f, 0.0f, 0.0f).Rotation ();

		for (int i = 0; i < _gridX [_currentIndex]; i++)
		{
			for (int j = 0; j < _gridY [_currentIndex]; j++)
			{
				//Spawn the unit
				AActor* gridSlot = world->SpawnActor <AActor> (_gridSlot, spawnPosition, rotator, spawnParams);
				_grid.Add (gridSlot);

				spawnPosition.Y += slotSize;
			}

			spawnPosition.Y -= (slotSize * _gridY [_currentIndex]);
			spawnPosition.X += slotSize;
		}
	}
}

void AMinigameSolarController::ControlUnit ()
{
	_controlledUnit = _soonUnit;
	_lifting = true;

	TArray <UStaticMeshComponent*> staticComps;
	_controlledUnit->GetComponents <UStaticMeshComponent> (staticComps);
	UStaticMeshComponent* _mesh = staticComps [0];
	_mesh->SetCollisionEnabled (ECollisionEnabled::NoCollision);

	/*for (int i = 0; i < _rotators.Num (); i++)
		_rotators [i]->Destroy ();

	_rotators.Empty ();*/

	_soonUnit = nullptr;

	UpdateBeams ();
}

void AMinigameSolarController::PlaceUnit ()
{
	AActor* closestGridSlot = nullptr;
		
	for (int i = 0; i < _grid.Num (); i++)
	{
		switch (_currentIndex)
		{
		case 0:
			if (i == 2 || i == 4 || i == 19 || i == 13 || i == 14)
				continue;
			break;
		case 1:
			if (i == 5 || i == 7 || i == 15 || i == 13 || i == 23)
				continue;
			break;
		case 2:
			if (i == 12 || i == 11 || i == 9 || i == 20 || i == 1 || i == 18 || i == 17)
				continue;
			break;
		case 3:
			if (i == 8 || i == 1 || i == 24 || i == 12 || i == 16 || i == 6)
				continue;
			break;
		}

		AActor* gridSlot = _grid [i];

		bool skip = false;

		for (int i = 0; i < _mirrors.Num (); i++)
		{

			if (FVector::Distance (_mirrors [i]->GetActorLocation (), gridSlot->GetActorLocation ()) < 5)
			{
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			if (!closestGridSlot)
				closestGridSlot = gridSlot;
			else if (FVector::Distance (gridSlot->GetActorLocation (), _controlledUnit->GetActorLocation ()) < FVector::Distance (closestGridSlot->GetActorLocation (), _controlledUnit->GetActorLocation ()))
				closestGridSlot = gridSlot;
		}
	}
		
	_controlledUnit->SetActorLocation (closestGridSlot->GetActorLocation ());

	//Spawn rotator
	/*FActorSpawnParameters spawnParams;
	FRotator rotator = FRotator (0.0f, 0.0f, 0.0f);

	UWorld* world = GetWorld ();

	for (int i = 0; i < _mirrorCount; i++)
	{
		FVector spawnPosition = _mirrors [i]->GetActorLocation () + FVector (0.0f, 0.0f, 100.0f);

		AActor* rotate = world->SpawnActor <AActor> (_rotatePrefab, spawnPosition, rotator, spawnParams);
		_rotators.Add (rotate);
	}*/

	TArray <UStaticMeshComponent*> staticComps;
	_controlledUnit->GetComponents <UStaticMeshComponent> (staticComps);
	UStaticMeshComponent* _mesh = staticComps [0];
	_mesh->SetCollisionEnabled (ECollisionEnabled::QueryAndPhysics);

	_lifting = false;
	_controlledUnit = nullptr;

	UpdateBeams ();
}

void AMinigameSolarController::RotateUnit (AActor* mirror)
{
	mirror->SetActorRotation (mirror->GetActorRotation () + FRotator (0.0f, 90.0f, 0.0f));

	if (_mirrorRotations [mirror] == 4)
		_mirrorRotations [mirror] = 1;
	else
		_mirrorRotations [mirror] = _mirrorRotations [mirror] + 1;

	UpdateBeams ();
}

void AMinigameSolarController::UpdateBeams ()
{
	//Remove all current beams
	for (int i = 0; i < _beams.Num (); i++)
		_beams [i]->Destroy ();

	_beams.Empty ();

	CheckBeamHit (_beamPosition->GetActorLocation (), _beamPosition->GetActorRotation (), true);
}

void AMinigameSolarController::CheckBeamHit (FVector position, FRotator rotation, bool firstBeam)
{
	//Spawn the beam
	FActorSpawnParameters spawnParams;
	FVector spawnPosition = position + FVector (0.0f, 00.0f, 50.0f);
	FRotator rotator = rotation;

	AActor* beam = GetWorld ()->SpawnActor <AActor> (_beam, spawnPosition, rotator, spawnParams);
	_beams.Add (beam);

	FHitResult hit;
	FVector start = beam->GetActorLocation ();

	if (firstBeam)
		_startBeam = beam;

	FVector forwardVector = beam->GetActorForwardVector ();
	FVector end = (forwardVector * 2000.0f) + start;
	FCollisionQueryParams collisionParams;

	TArray <AActor*> possibleMirrors;

	for (int i = 0; i < _mirrors.Num (); i++)
	{
		if (_mirrors [i] == _controlledUnit)
			continue;

		if (_mirrors [i]->ActorLineTraceSingle (hit, start + (forwardVector * 75), end, ECC_WorldDynamic, collisionParams))
			possibleMirrors.Add (_mirrors [i]);
	}

	for (int i = 0; i < _obstacles.Num (); i++)
	{
		if (_obstacles [i]->ActorLineTraceSingle (hit, start + (forwardVector * 75), end, ECC_WorldDynamic, collisionParams))
			possibleMirrors.Add (_obstacles [i]);
	}

	if (possibleMirrors.Num () > 0)
	{
		int closestMirrorIndex = 0;

		for (int i = 1; i < possibleMirrors.Num (); i++)
		{
			if (FVector::Distance (start, possibleMirrors [i]->GetActorLocation ()) < FVector::Distance (start, possibleMirrors [closestMirrorIndex]->GetActorLocation ()))
				closestMirrorIndex = i;
		}

		AActor* hitMirror = possibleMirrors [closestMirrorIndex];

		if (!hitMirror->GetRootComponent ()->ComponentHasTag ("Mirror"))
		{
			//Get the beam particle component
			TArray <UParticleSystemComponent*> staticComps;
			beam->GetComponents <UParticleSystemComponent> (staticComps);
			UParticleSystemComponent* beamComponent = staticComps [0];

			//Set beam start and end position
			beamComponent->SetActorParameter ("Source", beam);
			beamComponent->SetActorParameter ("Target", hitMirror);

			return;
		}

		//Get the beam particle component
		TArray <UParticleSystemComponent*> staticComps;
		beam->GetComponents <UParticleSystemComponent> (staticComps);
		UParticleSystemComponent* beamComponent = staticComps [0];

		//Set beam start and end position
		beamComponent->SetActorParameter ("Source", beam);
		beamComponent->SetActorParameter ("Target", hitMirror);

		//Mirror rotation
		int rot = rotation.Yaw;

		switch (_mirrorRotations [hitMirror])
		{
		case 1:
			if (rot < 5 && rot > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, -90.0f, 0.0f), false);
			else if (rot - 90 < 5 && rot - 90 > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, 180.0f, 0.0f), false);
			break;
		case 2:
			if (rot - 90 < 5 && rot - 90 > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, 0.0f, 0.0f), false);
			else if (rot - 180 < 5 && rot - 180 > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, -90.0f, 0.0f), false);
			break;
		case 3:
			if (rot + 90 < 5 && rot + 90 > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, 0.0f, 0.0f), false);
			else if (rot - 180 < 5 && rot - 180 > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, 90.0f, 0.0f), false);
			break;
		case 4:
			if (rot < 5 && rot > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, 90.0f, 0.0f), false);
			else if (rot + 90 < 5 && rot + 90 > -5)
				CheckBeamHit (hitMirror->GetActorLocation (), FRotator (0.0f, 180.0f, 0.0f), false);
			break;
		}
	}
	else
	{
		if (_goalPosition->ActorLineTraceSingle (hit, start + (forwardVector * 75), end, ECC_WorldDynamic, collisionParams))
		{
			//Get the beam particle component
			TArray <UParticleSystemComponent*> staticComps;
			beam->GetComponents <UParticleSystemComponent> (staticComps);
			UParticleSystemComponent* beamComponent = staticComps [0];

			//Set beam start and end position
			beamComponent->SetActorParameter ("Source", beam);
			beamComponent->SetActorParameter ("Target", _goalTarget);

			EndGame ();
		}
	}
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

	//Enable start UI
	_uiController->Enable (17, 0);

	InitializeGame ();
}

void AMinigameSolarController::StartGamePlay ()
{
	_gameStarted = true;

	//Disable start UI
	_uiController->Disable (17);

	//Enable solarMiniGame UI
	_uiController->Enable (20, 0);

	UpdateBeams ();
}

void AMinigameSolarController::EndGame ()
{
	SetScore ((int) _timer);
	endScoreText = "1/1";

	_endTime = (int) _timer;
	_timer = 60.0f;

	//Disable solarMiniGame UI
	_uiController->Disable (20);

	//Enable lose panel UI
	//_uiController->Enable (19, 2);
	//Enable win panel UI
	_uiController->Enable (19, 2);

	_animatingStars = true;
	
	_endPanelShown = true;
	_gameStarted = false;
	_gameController->StartClickDelay ();
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

	//Clean up everything
	_beamPosition->Destroy ();
	_goalPosition->Destroy ();
	_goalTarget->Destroy ();
	_startBeam->Destroy ();

	for (int i = 0; i < _grid.Num (); i++)
		_grid [i]->Destroy ();

	_grid.Empty ();

	for (int i = 0; i < _mirrors.Num (); i++)
		_mirrors [i]->Destroy ();

	_mirrors.Empty ();
	_mirrorRotations.Empty ();

	for (int i = 0; i < _beams.Num (); i++)
		_beams [i]->Destroy ();

	_beams.Empty ();

	for (int i = 0; i < _obstacles.Num (); i++)
		_obstacles [i]->Destroy ();

	_obstacles.Empty ();

	/*for (int i = 0; i < _rotators.Num (); i++)
		_rotators [i]->Destroy ();

	_rotators.Empty ();*/
}

void AMinigameSolarController::OnMouseClick ()
{
	if (_endPanelShown)
	{
		if (_gameController->CanContinue ())
			GoBackToSimulation ();
	}
	else if (!_gameStarted)
		StartGamePlay ();
	else if (!_controlledUnit)
	{
		FHitResult hit;
		GetWorld ()->GetFirstPlayerController ()->GetHitResultUnderCursor (ECC_Visibility, true, hit);

		if (hit.bBlockingHit)
		{
			if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Mirror"))
			{
				_pressing = true;
				_soonUnit = hit.GetActor ();
			}
		}
	}

	if (_gameController->GetInOptions ())
		_gameController->GoBackToGame ();
}

void AMinigameSolarController::OnMouseRelease ()
{
	if (_lifting)
		PlaceUnit ();

	if (_pressing)
	{
		FHitResult hit;
		GetWorld ()->GetFirstPlayerController ()->GetHitResultUnderCursor (ECC_Visibility, true, hit);

		if (hit.bBlockingHit)
		{
			if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Rotate"))
				ControlUnit ();
			else if (hit.GetActor ()->GetRootComponent ()->ComponentHasTag ("Mirror"))
			{
				for (int i = 0; i < _mirrors.Num (); i++)
				{
					if (hit.GetActor () == _mirrors [i])
					{
						if (_pressingTimer < _delayTime)
							RotateUnit (_mirrors [i]);

						break;
					}
				}
			}
		}

		_pressing = false;
		_pressingTimer = 0.0f;
	}
}

//Called to bind functionality to input
void AMinigameSolarController::SetupPlayerInputComponent (UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent (PlayerInputComponent);

	PlayerInputComponent->BindAction ("MouseClick", IE_Pressed, this, &AMinigameSolarController::OnMouseClick);
	PlayerInputComponent->BindAction ("MouseClick", IE_Released, this, &AMinigameSolarController::OnMouseRelease);
}
