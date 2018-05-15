// Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameCartController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

// Sets default values
AMinigameCartController::AMinigameCartController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AMinigameCartController::BeginPlay()
{
	Super::BeginPlay();
	setup();
}

void AMinigameCartController::setup() {

	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create a camera and a visible object
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));

	// Attach our camera and visible object to our root component. Offset and rotate the camera.
	OurCamera->SetupAttachment(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

	UWorld* world = GetWorld();
	if (world) {
		FVector Location(0, 0, 0);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		//SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		_spawnedWagon = world->SpawnActor<AMinecart>(wagonPrefab, Location, Rotation, SpawnInfo);
		print("Spawned Wagon");
	}
}

// Called every frame
void AMinigameCartController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (!_velocity.IsZero() && IsValid(_spawnedWagon)) {
		FVector NewLocation = _spawnedWagon->GetActorLocation() + (_velocity * DeltaTime);
		_spawnedWagon->SetActorLocation(NewLocation);
	}*/

}

void AMinigameCartController::handleInput(float axisValue) {
	_velocity.X = FMath::Clamp(axisValue, -1.0f, 1.0f) * 100.0f;
}

// Called to bind functionality to input
void AMinigameCartController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("HorizontalAxis", this, &AMinigameCartController::handleInput);

}

