// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class PROJECTEXPOSURE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY ()
	
public:
	AMainPlayerController ();

protected:
	//Begin PlayerController interface
	virtual void PlayerTick (float deltaTime) override;
	virtual void SetupInputComponent () override;

	void PlaceUnit ();
};
