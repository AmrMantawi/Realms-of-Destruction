// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterMovement.h"
#include "Bot.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API ABot : public ACharacterMovement
{
	GENERATED_BODY()

public: 
	ABot();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void StartJump() override;

	// Fly downwards
	virtual void StopJump() override;

	virtual void StartSprint() override;

	// Clears sprint flag when key is released.
	virtual void StopSprint() override;


	// Sets sprint flag when key is pressed.
	virtual void Server_StartSprint_Implementation() override;


	// Clears sprint flag when key is released.
	virtual void Server_StopSprint_Implementation() override;

	UFUNCTION()
	void Fall();

private:
	UPROPERTY(Replicated)
		float FlySpeed;
};
