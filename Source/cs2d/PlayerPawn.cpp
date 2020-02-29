// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Player = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	UCameraComponent *Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Body->SetupAttachment(Player);

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule"));

	if (MeshAsset.Succeeded())
	{
		Body->SetStaticMesh(MeshAsset.Object);
	}

	Weapon->SetRelativeScale3D(FVector(0.75f, 0.1f, 0.1f));
	Weapon->SetRelativeLocation(FVector(15.0f, 15.0f, 50.0f));
	Weapon->SetupAttachment(Body);
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 500.0f));
	Camera->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Camera->SetupAttachment(Player);
	Player->SetupAttachment(RootComponent);
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
}
