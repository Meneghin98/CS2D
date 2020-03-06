// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SetRootComponent(Body);

	ConstructorHelpers::FObjectFinder<UStaticMesh> BodyAsset(TEXT("/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> GunAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (BodyAsset.Succeeded() && GunAsset.Succeeded())
	{
		Body->SetStaticMesh(BodyAsset.Object);
		Weapon->SetStaticMesh(GunAsset.Object);
	}

	Weapon->SetRelativeScale3D(FVector(0.75f, 0.1f, 0.1f));
	Weapon->SetRelativeLocation(FVector(15.0f, 15.0f, 50.0f));
	Weapon->SetupAttachment(Body);
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 500.0f));
	Camera->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Camera->SetupAttachment(Body);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveUP", this, &APlayerPawn::MoveX);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveY);
}

void APlayerPawn::MoveX(float AxisValue) {
	MovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
}

void APlayerPawn::MoveY(float AxisValue) {
	MovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
}