//Fill out your copyright notice in the Description page of Project Settings.

#include "CameraMovement.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

CameraMovement::CameraMovement (AActor* cameraActor) : _cameraActor (cameraActor)
{

}

void CameraMovement::Update (float deltaTime)
{
	if (!_targetPosition.IsZero ())
		MoveCamera (deltaTime);
}

void CameraMovement::MoveTo (FVector targetPosition, FVector targetRotation)
{
	_startPosition = _cameraActor->GetActorLocation ();

	_targetPosition = targetPosition;
	_targetRotation = FRotator (targetRotation.Y, targetRotation.Z, targetRotation.X);

	_moving = true;
}

FVector CameraMovement::GetTargetPosition ()
{
	return _targetPosition;
}

void CameraMovement::MoveCamera (float deltaTime)
{
	FVector currentPosition = _cameraActor->GetActorLocation ();

	float camDistToPoint = FVector::Distance (currentPosition, _targetPosition);
	float distBetweenPoints = FVector::Distance (_startPosition, _targetPosition);

	//camDistToPoint = FVector::Distance (_startPosition, _targetPosition);;

	float speed = FMath::Lerp (4.0f, 1.0f, camDistToPoint / distBetweenPoints) * deltaTime / 1.5f;

	_cameraActor->SetActorLocation (FMath::Lerp (currentPosition, _targetPosition, speed));
	_cameraActor->SetActorRotation (FMath::Lerp (_cameraActor->GetActorRotation (), _targetRotation, speed / 2.0f));
}

CameraMovement::~CameraMovement ()
{

}
