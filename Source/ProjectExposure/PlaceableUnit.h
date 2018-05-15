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

	bool PlaceUnit ();
	void SetMaterials (UMaterialInterface* normalMaterial, UMaterialInterface* collideMaterial);

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay () override;
	
private:
	void FollowMousePosition ();

	APlayerController* _playerController;

	bool _isPlaced = false;

	UStaticMeshComponent* _mesh;

	UFUNCTION ()
	void BeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	//Materials
	UMaterialInterface* _normalMaterial;
	UMaterialInterface* _collideMaterial;
};
