// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//When placed on the level it gets the control of the player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Helps move the pawn
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

	//Component to setup the mesh of the body and the gun(it will be paper2d in the future)
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	SetRootComponent(Body);

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(Body);

	//Component needed to help the camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = false;
	
	//Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	//Setup of the meshes
	ConstructorHelpers::FObjectFinder<UStaticMesh> BodyAsset(TEXT("/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> GunAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (BodyAsset.Succeeded() && GunAsset.Succeeded())
	{
		Body->SetStaticMesh(BodyAsset.Object);
		Weapon->SetStaticMesh(GunAsset.Object);
	}

	//Setup relative location and rotation
	Weapon->SetRelativeScale3D(FVector(0.75f, 0.1f, 0.1f));
	Weapon->SetRelativeLocation(FVector(15.0f, 15.0f, 50.0f));
	CameraBoom->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController>(GetController());
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController != nullptr)
	{
		FHitResult TraceResult(ForceInit);
		PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, TraceResult);

		FRotator Target = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TraceResult.Location);
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), Target, DeltaTime, 33.f);
		SetActorRotation(FRotator(0.f, Target.Yaw, 0.f));
	}
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
	//MovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
	MovementComponent->AddInputVector(FVector(1.f,0.f,0.f) * AxisValue);
}

void APlayerPawn::MoveY(float AxisValue) {
	//MovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
	MovementComponent->AddInputVector(FVector(0.f,1.f,0.f) * AxisValue);
}