//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "PlaceableUnit.generated.h"

UCLASS()
class PROJECTEXPOSURE_API APlaceableUnit : public APawn
{
	GENERATED_BODY ()

public:
	//Sets default values for this pawn's properties
	APlaceableUnit ();

	//Called every frame
	virtual void Tick (float deltaTime) override;

	void ActivateOutline (bool status);
	void SetRemovable (bool status);
	bool GetRemovable ();
	bool PlaceUnit ();
	int GetTypeIndex ();

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;
	
private:
	void FollowMousePosition ();
	void ActivateRemoveOutline ();

	APlayerController* _playerController;

	bool _isPlaced = false;
	bool _isRemovable = false;

	UPROPERTY () UStaticMeshComponent* _mesh;
};
