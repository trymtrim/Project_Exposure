// Fill out your copyright notice in the Description page of Project Settings.

#include "FallingUnit.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

// Sets default values
AFallingUnit::AFallingUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFallingUnit::BeginPlay()
{
	Super::BeginPlay();
}

void AFallingUnit::init(UnitType pType, float pSpeed, float pDeathThreshold) {
	_currentType = pType;
	_speed = pSpeed;
	_deathThreshhold = pDeathThreshold;

	TArray<UStaticMeshComponent*> Components;
	GetComponents<UStaticMeshComponent>(Components);

	//Set type and mesh accordingly
	if (_currentType == UnitType::DEBRIS) {
		for (int32 i = 0; i<Components.Num(); i++) {
			UStaticMeshComponent* StaticMeshComponent = Components[i];
			StaticMeshComponent->SetStaticMesh(debrisMesh);
		}
	} else {
		for (int32 i = 0; i<Components.Num(); i++) {
			UStaticMeshComponent* StaticMeshComponent = Components[i];
			StaticMeshComponent->SetStaticMesh(uraniumMesh);
		}
	}
}

UnitType AFallingUnit::getType() {
	return _currentType;
}

//For more info look into FallinUnit Blueprint
void AFallingUnit::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
	Destroy();
}

// Called every frame
void AFallingUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (FVector(0.0f, 0.0f, -_speed * 100.0f) * DeltaTime);
	SetActorLocation(NewLocation);

	if (NewLocation.Z < _deathThreshhold) Destroy();
}

