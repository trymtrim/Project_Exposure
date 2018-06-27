//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UIController.h"
#include "CameraMovement.h"
#include "IntroController.generated.h"

UCLASS()
class PROJECTEXPOSURE_API AIntroController : public APawn
{
	GENERATED_BODY ()

public:
	//Sets default values for this pawn's properties
	AIntroController ();

	//Called every frame
	virtual void Tick (float DeltaTime) override;

	//Called to bind functionality to input
	virtual void SetupPlayerInputComponent (class UInputComponent* PlayerInputComponent) override;

	//For blueprints
	UFUNCTION (BlueprintImplementableEvent, Category = "ChangeLevel")
	void ChangeLevel ();
	UFUNCTION (BlueprintImplementableEvent, Category = "Video")
	void Pause ();
	UFUNCTION (BlueprintImplementableEvent, Category = "Video")
	void Resume ();

	UFUNCTION (BlueprintCallable)
	void StartGame ();

	UFUNCTION (BlueprintImplementableEvent, Category = "Video")
	void RestartLevel ();

	UPROPERTY (EditAnywhere, BlueprintReadOnly)
	AUIController* uiController;

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	void OnMouseClick ();
	void UpdateTimer (float deltaTime);
	void MoveToWindow ();
	void StartVideo ();
	void PauseVideo ();
	void ContinueVideo ();
	void ExitVideo ();
	void QuitGame ();

	bool _startingGame = false;

	float _startTimer = 0.0f;
	float _videoTimer = 0.0f;

	//Fading
	void FadeIn (float delayTime, float fadeTime);
	void FadeOut (float delayTime, float fadeTime);
	void UpdateFading (float deltaTime);

	float _fadeInTimer;
	float _fadeOutTimer;
	float _fadeInDelay;
	float _fadeOutDelay;
	bool _fadingIn = false;
	bool _fadingOut = false;

	//UI
	bool _uiEnabled = false;

	CameraMovement* _cameraMovement;

	enum State
	{
		MENU,
		CAMERAMOVING,
		FADING,
		VIDEO,
		PAUSE,
		AFTERPAUSE,
		EXITING
	};

	State _currentState = MENU;
};
