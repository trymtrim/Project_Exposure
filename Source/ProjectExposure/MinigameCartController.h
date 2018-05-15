// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "MinigameCart/Minecart.h"
#include "MinigameCartController.generated.h"

UCLASS()
class PROJECTEXPOSURE_API AMinigameCartController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMinigameCartController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> wagonPrefab;
	
private:

	void setup();
	void handleInput(float axisValue);

	UPROPERTY()
	AMinecart* _spawnedWagon;

	UPROPERTY()
	FVector _velocity;
};
