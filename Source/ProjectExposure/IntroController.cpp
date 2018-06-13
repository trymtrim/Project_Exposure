//Fill out your copyright notice in the Description page of Project Settings.

#include "IntroController.h"
#include "Engine/World.h"
#include "CustomGameViewportClient.h"

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
		//Enable menu UI
		uiController->Enable (3, 0);

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

	UpdateTimer (DeltaTime);
}

void AIntroController::OnMouseClick ()
{
	switch (_currentState)
	{
	case MENU:
		MoveToWindow ();
		break;
	case PAUSE:
		ContinueVideo ();
		break;
	}
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
		{
			delete _cameraMovement;
			ChangeLevel ();
		}
	}
}

void AIntroController::MoveToWindow ()
{
	//Disable menu UI
	uiController->Disable (3);

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

//Called to bind functionality to input
void AIntroController::SetupPlayerInputComponent (UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent (PlayerInputComponent);

	PlayerInputComponent->BindAction ("MouseClick", IE_Pressed, this, &AIntroController::OnMouseClick);
	PlayerInputComponent->BindAction ("R", IE_Pressed, this, &AIntroController::RestartLevel);
	PlayerInputComponent->BindAction ("S", IE_Pressed, this, &AIntroController::ChangeLevel);
}

