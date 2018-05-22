//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class PROJECTEXPOSURE_API CameraMovement
{
public:
	CameraMovement (AActor* cameraActor);
	~CameraMovement ();

	void Update (float deltaTime);
	void MoveTo (FVector targetPosition, FVector targetRotation);
	FVector GetTargetPosition ();

private:
	void MoveCamera (float deltaTime);

	FVector _startPosition;
	FVector _targetPosition = FVector (0.0f, 0.0f, 0.0f);
	FRotator _targetRotation;

	AActor* _cameraActor;

	bool _moving = false;
};
