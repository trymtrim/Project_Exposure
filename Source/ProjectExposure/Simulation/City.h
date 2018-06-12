// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "City.generated.h"

UCLASS()
class PROJECTEXPOSURE_API ACity : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleStage(int pStage);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray< AActor* > _stageOne;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray< AActor* > _stageTwo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray< AActor* > _stageThree;

private:
	
	void LerpStages(float DeltaTime);

	UPROPERTY(EditAnywhere)
	UMaterialInterface* _parentMaterial;

	UPROPERTY()
	TArray<bool> _stageActives;

	UPROPERTY()
	TArray<float> _stageLerps;
};
