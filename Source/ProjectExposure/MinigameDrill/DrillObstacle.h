//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrillObstacle.generated.h"

class AMinigameDrillController;

UCLASS()
class PROJECTEXPOSURE_API ADrillObstacle : public AActor
{
	GENERATED_BODY ()
	
public:	
	//Sets default values for this actor's properties
	ADrillObstacle ();

	//Called every frame
	virtual void Tick (float DeltaTime) override;

	void UpdateMovement (float deltaTime);
	void Initialize (AActor* drill, AMinigameDrillController* gameController);

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;

private:
	int _type;

	AMinigameDrillController* _gameController;

	float _collideHeight;

	bool _hasCollided = false;
};
