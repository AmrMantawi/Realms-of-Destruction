// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "CharacterMovement.generated.h"

UCLASS()
class REALMSOFDESTRUCTION_API ACharacterMovement : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterMovement();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void Setup();

	UFUNCTION(Client, Reliable)
	void Client_Setup();
	void Client_Setup_Implementation();

	//Toggle Character Selection Menu
	UFUNCTION()
	void ToggleCharacterSelectionMenu();

	//Toggle Pause Menu
	UFUNCTION()
	void TogglePauseMenu();

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY()
		class UPauseMenu* playerPauseMenu;

	UPROPERTY()
		class UCharacterSelectionMenu* playerCharacterSelection;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCharacterSelectionMenu> characterSelectionClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UPauseMenu> PauseMenuClass;

	UPROPERTY()
		bool pauseMenuDisplayed = false;

	UPROPERTY()
		bool characterSelectionMenuDisplayed = false;

	UFUNCTION()
	void MenuMode();

	UFUNCTION()
	void PlayMode();

private:

	void Die();

	FTimerHandle timerHandle;


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

	UPROPERTY()
	float bonusSpeed = 1;

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

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

	void MultiplySpeed(float value);

	UFUNCTION(Reliable, Client)
	void Client_SetHealth(float currentPlayerHealth);
	void Client_SetHealth_Implementation(float currentPlayerHealth);

	UFUNCTION(Reliable, Client)
	void Client_SetShield(float currentPlayerShield);
	void Client_SetShield_Implementation(float currentPlayerShield);

	UFUNCTION(Reliable, Client)
	void Client_SetSpeed(float playerSpeedMultiplier);
	void Client_SetSpeed_Implementation(float playerSpeedMultiplier);

	UFUNCTION(Reliable, Client)
	void Client_ResetSpeed();
	void Client_ResetSpeed_Implementation();

	UPROPERTY(EditAnywhere)
	int maxHealth = 420;

	UPROPERTY(EditAnywhere)
	int maxShield = 100.0f;

	UPROPERTY()
	int currentHealth;
	UPROPERTY()
	int currentShield;

	UPROPERTY()
	class AGamePlayerController* GamePlayerController;

	AGamePlayerController* PC;
};