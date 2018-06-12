// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallingUnit.generated.h"

UENUM(BlueprintType)
enum class UnitType : uint8 {
	URANIUM,
	DEBRIS
};

UCLASS()
class PROJECTEXPOSURE_API AFallingUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallingUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Collision, for more info look into FallinUnit Blueprint
	UFUNCTION(BlueprintCallable, Category = "CustomCollisionHandlers")
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	//Called from the MinigameController to set vars
	void init(UnitType pType, float pSpeed, float pDeathThreshold);

	//Called from the Minecart to determine type
	UnitType getType();

	//Meshes for different UnitTypes
	UPROPERTY(EditAnywhere)
	UStaticMesh* uraniumMesh;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* debrisMesh;

private:
	UPROPERTY(VisibleAnywhere)
	UnitType _currentType;

	UPROPERTY()
	float _speed;

	UPROPERTY()
	float _deathThreshhold;

};
