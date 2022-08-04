// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "CharacterMovement.generated.h"

UCLASS()
class UNREALENGINEGAME_API ACharacterMovement : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterMovement();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Setup();

private:
	~ACharacterMovement();

	UPROPERTY(EditAnywhere)
		float maxHealth;

	UPROPERTY(EditAnywhere)
		float maxShield;

	void Die();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handles input for moving forward and backward.
	UFUNCTION()
		void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
		void MoveRight(float Value);

	// Sets jump flag when key is pressed.
	UFUNCTION()
		void StartJump();

	// Clears jump flag when key is released.
	UFUNCTION()
		void StopJump();

	// Sets sprint flag when key is pressed.
	UFUNCTION()
		void StartSprint();

	// Clears sprint flag when key is released.
	UFUNCTION()
		void StopSprint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float speedMultiplier = 2;

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCameraComponent;

	// Function that handles firing projectiles.
	UFUNCTION()
		void Fire();

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	//UPROPERTY(EditDefaultsOnly, Category = Projectile)
		
	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UHealthBar> HealthBarClass;

	UPROPERTY()
		class UHealthBar* PlayerHealthBar;

	//Take Damage
	void CharacterTakeDamage(float value);

	//Damage To Health
	void DamageHealth(float value);

	//Damage to Sheild
	void DamageShield(float value);

	//Gain Health
	void GainHealth(float value);

	//Gain Sheild
	void GainShield(float value);

	float currentHealth;
	float currentShield;

	UPROPERTY()
		class AGamePlayerController* GamePlayerController;

	AGamePlayerController* PC;
};