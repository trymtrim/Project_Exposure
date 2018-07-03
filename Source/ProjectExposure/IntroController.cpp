//Fill out your copyright notice in the Description page of Project Settings.

#include "IntroController.h"
#include "Engine/World.h"
#include "CustomGameViewportClient.h"
#include "MainGameInstance.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

//Sets default values
AIntroController::AIntroController ()
{
 	//Set this pawn to call Tick () every frame
	PrimaryActorTick.bCanEverTick = true;
}

//Called when the game starts or when spawned
void AIntroController::BeginPlay ()
{
	Super::BeginPlay ();

	_cameraMovement = new CameraMovement (this);
}

//Called every frame
void AIntroController::Tick (float DeltaTime)
{
	Super::Tick (DeltaTime);

	if (!_uiEnabled)
	{
		if (Cast <UMainGameInstance> (GetWorld ()->GetGameInstance ())->Restart)
		{
			Cast <UMainGameInstance> (GetWorld ()->GetGameInstance ())->Restart = false;

			_restartingGame = true;
		}
		else
		{
			//Enable menu UI
			uiController->Enable (3, 0);
		}

		_uiEnabled = true;
	}

	if (_fadingIn || _fadingOut)
		UpdateFading (DeltaTime);

	if (FVector::Distance (GetActorLocation (), _cameraMovement->GetTargetPosition ()) > 5.0f)
		_cameraMovement->Update (DeltaTime);

	if (FVector::Distance (GetActorLocation (), _cameraMovement->GetTargetPosition ()) < 5.0f && _currentState == CAMERAMOVING)
	{
		/*FadeIn (0.0f, 0.5f);
		FadeOut (1.5f, 0.5f);

		_currentState = FADING;*/
		
		StartVideo ();
	}

	if (_startingGame)
	{
		_startTimer += DeltaTime;

		if (_startTimer >= 1.0f)
		{
			_startingGame = false;
			MoveToWindow ();
		}
	}

	if (_restartingGame)
	{
		_restartGameTimer += DeltaTime;

		if (_restartGameTimer >= 0.3f)
		{
			_restartGameTimer = 0.0f;
			_restartingGame = false;

			StartGame ();
		}
	}

	UpdateTimer (DeltaTime);
}

void AIntroController::StartGame ()
{
	if (!Cast <UMainGameInstance> (GetWorld ()->GetGameInstance ())->Restart)
	{
		//Disable menu UI
		uiController->Disable (3);
	}

	//Enable reset button UI
	//uiController->Enable (5, 3);

	//Enable keyboard UI
	uiController->Enable (7, 0);

	_keyboard = true;

	UpdateName ();
}

void AIntroController::FinishName ()
{
	if (playerName.Len () == 0)
		return;

	//Disable keyboard UI
	uiController->Disable (7);

	_startingGame = true;
	
	Cast <UMainGameInstance> (GetWorld ()->GetGameInstance ())->PlayerName = playerName;
}

void AIntroController::AddLetter (FString letter)
{
	if (letter == "BACKSPACE")
	{
		if (playerName.Len () > 0)
			playerName.RemoveAt (playerName.Len () - 1);
	}
	else if (playerName.Len () < 15)
		playerName += letter;
}

void AIntroController::OnMouseClick ()
{
	if (_currentState == PAUSE)
		ContinueVideo ();
}

void AIntroController::UpdateTimer (float deltaTime)
{
	if (_currentState == FADING)
	{
		_videoTimer += deltaTime;

		if (_videoTimer >= 1.5f)
		{
			_videoTimer = 0.0f;
			StartVideo ();
		}
	}
	else if (_currentState == VIDEO)
	{
		_videoTimer += deltaTime;

		//Maybe change to "on video end"
		if (_videoTimer >= 17.18f)
		{
			_videoTimer = 0.0f;
			PauseVideo ();
		}
	}
	else if (_currentState == AFTERPAUSE)
	{
		_videoTimer += deltaTime;

		if (_videoTimer >= 3.0f)
		{
			_videoTimer = 0.0f;
			ExitVideo ();
		}
	}
	else if (_currentState == EXITING)
	{
		_videoTimer += deltaTime;

		if (_videoTimer >= 1.0f)
			GoToGameLevel ();
	}
}

void AIntroController::GoToGameLevel ()
{
	delete _cameraMovement;
	ChangeLevel ();
}

void AIntroController::MoveToWindow ()
{
	FVector targetPosition = FVector (-2510, -3357, 1018);
	FVector targetRotation = FVector (0, -4, 180);

	_cameraMovement->MoveTo (targetPosition, targetRotation);

	_currentState = CAMERAMOVING;
}

void AIntroController::StartVideo ()
{
	//Enable video UI
	uiController->Enable (14, 0);

	_currentState = VIDEO;
}

void AIntroController::PauseVideo ()
{
	//Call Pause for blueprint
	Pause ();

	//Enable videoPause UI
	//uiController->Enable (15, 1);

	_currentState = PAUSE;
}

void AIntroController::ContinueVideo ()
{
	//If the video-buttuon is pressed
	APlayerController* playerController = GetWorld ()->GetFirstPlayerController ();
	FVector2D mousePos;
	playerController->GetMousePosition (mousePos.X, mousePos.Y);

	FIntVector viewportBounds;
	playerController->GetViewportSize (viewportBounds.X, viewportBounds.Y);

	FVector2D percentage = FVector2D (mousePos.X / viewportBounds.X * 100, mousePos.Y / viewportBounds.Y * 100);

	if (percentage.X >= 81 && percentage.X <= 93 && percentage.Y >= 26 && percentage.Y <= 50)
	{
		//Call Resume for blueprint
		Resume ();

		//Disable videoPause UI
		//uiController->Disable (15);

		_currentState = AFTERPAUSE;
	}
}

void AIntroController::ExitVideo ()
{
	FadeIn (0.0f, 1.0f);

	_currentState = EXITING;
}

void AIntroController::FadeIn (float delayTime, float fadeTime)
{
	_fadeInTimer = delayTime;
	_fadeInDelay = fadeTime;
	_fadingIn = true;
}

void AIntroController::FadeOut (float delayTime, float fadeTime)
{
	_fadeOutTimer = delayTime;
	_fadeOutDelay = fadeTime;
	_fadingOut = true;
}

void AIntroController::UpdateFading (float deltaTime)
{
	if (_fadingIn)
	{
		_fadeInTimer -= deltaTime;

		if (_fadeInTimer < 0.0f)
		{
			UCustomGameViewportClient* GameViewportClient = Cast <UCustomGameViewportClient> (GetWorld ()->GetGameViewport ());

			if (GameViewportClient)
			{
				GameViewportClient->Fade (_fadeInDelay, true);
				_fadingIn = false;
			}
		}
	}

	if (_fadingOut)
	{
		_fadeOutTimer -= deltaTime;

		if (_fadeOutTimer < 0.0f)
		{
			UCustomGameViewportClient* GameViewportClient = Cast <UCustomGameViewportClient> (GetWorld ()->GetGameViewport ());

			if (GameViewportClient)
			{
				GameViewportClient->Fade (_fadeOutDelay, false);
				_fadingOut = false;
			}
		}
	}
}

void AIntroController::QuitGame ()
{
	delete _cameraMovement;
	RestartLevel ();
}

//Called to bind functionality to input
void AIntroController::SetupPlayerInputComponent (UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent (PlayerInputComponent);

	PlayerInputComponent->BindAction ("MouseClick", IE_Pressed, this, &AIntroController::OnMouseClick);
}
