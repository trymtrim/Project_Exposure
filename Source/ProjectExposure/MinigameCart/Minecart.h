// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Minecart.generated.h"

class AMinigameCartController;

UCLASS()
class PROJECTEXPOSURE_API AMinecart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinecart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Collisions
	UFUNCTION(BlueprintCallable, Category="CustomCollisionHandlers")
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	//So that we have a reference to the MinigameController
	void setController(AMinigameCartController* pController);
private:
	
	UPROPERTY()
	AMinigameCartController* _controller;
};
