//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MinigameSolarController.generated.h"

class ASimulationGameController;
class AUIController;

UCLASS()
class PROJECTEXPOSURE_API AMinigameSolarController : public APawn
{
	GENERATED_BODY ()

public:
	//Sets default values for this pawn's properties
	AMinigameSolarController ();

	//Called every frame
	virtual void Tick (float DeltaTime) override;

	//Called to bind functionality to input
	virtual void SetupPlayerInputComponent (class UInputComponent* PlayerInputComponent) override;

	void SetGameController (ASimulationGameController* gameController);
	void StartGame ();

	//For blueprints
	UPROPERTY (BlueprintReadOnly)
	FString scoreText = "0";
	UPROPERTY (BlueprintReadOnly)
	FString endScoreText = "";

	UFUNCTION (BlueprintImplementableEvent, Category = "SolarUpdate")
	void Update ();
	UFUNCTION (BlueprintImplementableEvent, Category = "SolarEnd")
	void EndScreen ();

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	void StartGamePlay ();
	void EndGame ();
	void SetScore (int score);
	void GoBackToSimulation ();
	void OnMouseClick ();

	ASimulationGameController* _gameController;

	int _score = 0;

	bool _gameStarted = false;
	bool _endPanelShown = false;

	//UI controller
	UPROPERTY (EditAnywhere)
	AUIController* _uiController;
};
