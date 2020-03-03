// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class CS2D_API APlayerPawn : public APawn
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Weapon;

	UPROPERTY(EditAnywhere)
	USceneComponent* Player;

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector2D Direction;
	float SpeedMultiplier;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetSpeedMultiplier(float NewSpeed) { SpeedMultiplier = NewSpeed; };
	FORCEINLINE float GetSpeedMultiplier() { return SpeedMultiplier; };

	void MoveX(float value);
	void MoveY(float value);
};
