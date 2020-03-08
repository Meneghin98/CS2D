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

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	SetRootComponent(Collision);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(Body);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	ConstructorHelpers::FObjectFinder<UStaticMesh> BodyAsset(TEXT("/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> GunAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (BodyAsset.Succeeded() && GunAsset.Succeeded())
	{
		Body->SetStaticMesh(BodyAsset.Object);
		Weapon->SetStaticMesh(GunAsset.Object);
	}


	Weapon->SetRelativeScale3D(FVector(0.75f, 0.1f, 0.1f));
	Weapon->SetRelativeLocation(FVector(15.0f, 15.0f, 50.0f));
	//Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 500.0f));
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
	MovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
}

void APlayerPawn::MoveY(float AxisValue) {
	MovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
}