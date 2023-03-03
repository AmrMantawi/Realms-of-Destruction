// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "CharacterMovement.generated.h"

UENUM()
enum class ECharacterState
{
	Normal,
	Stunned,
	Dead
};

UCLASS()
class REALMSOFDESTRUCTION_API ACharacterMovement : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterMovement();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;




	UFUNCTION()
	void Setup();

	UFUNCTION(Client, Reliable)
	void Client_Setup();
	void Client_Setup_Implementation();

	UFUNCTION(Client, Reliable)
	void Client_UnSetup();
	void Client_UnSetup_Implementation();

	//Toggle Character Selection Menu
	UFUNCTION()
	void ToggleCharacterSelectionMenu();

	//Toggle Pause Menu
	UFUNCTION()
	void TogglePauseMenu();

	UFUNCTION()
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION()
	virtual void UnPossessed() override;

	UPROPERTY()
		class UPauseMenu* playerPauseMenu;

	UPROPERTY()
		class UCharacterSelectionMenu* playerCharacterSelection;
	UPROPERTY()
	class UPlayesrDisplay* playersMenu;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCharacterSelectionMenu> characterSelectionClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UPauseMenu> PauseMenuClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UPlayesrDisplay> PlayersMenuClass;

	UPROPERTY()
		bool pauseMenuDisplayed = false;

	UPROPERTY()
		bool characterSelectionMenuDisplayed = false;

	UPROPERTY()
		bool playersMenuDisplayed = false;

	UFUNCTION()
		void TogglePlayersMenu();

	UFUNCTION()
	void MenuMode();

	UFUNCTION()
	void PlayMode();

	UPROPERTY()
	class USettingsMenu* playerSettingsMenu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class USettingsMenu> SettingsMenuClass;

	UPROPERTY()
	bool settingsDisplayed = false;

	UFUNCTION()
	void ToggleSettingsMenu();

	UFUNCTION()
	void OnRep_HandleCharacterState();

	UFUNCTION()
	void setCharacterState(ECharacterState state);

	UFUNCTION()
	ECharacterState getCharacterState();


private:
	
	UPROPERTY(ReplicatedUsing = OnRep_HandleCharacterState)
		ECharacterState characterState;

	UFUNCTION()
		void Die();

	UFUNCTION()
	void DeadState();

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
		virtual void StartJump();

	// Clears jump flag when key is released.
	UFUNCTION()
		virtual void StopJump();

	// Sets sprint flag when key is pressed.
	UFUNCTION()
		virtual void StartSprint();

	// Clears sprint flag when key is released.
	UFUNCTION()
		virtual void StopSprint();


	// Sets sprint flag when key is pressed.
	UFUNCTION(Server, Reliable)
		virtual void Server_StartSprint();
		virtual void Server_StartSprint_Implementation();


	// Clears sprint flag when key is released.
	UFUNCTION(Server, Reliable)
		virtual void Server_StopSprint();
		virtual void Server_StopSprint_Implementation();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float speedMultiplier = 2;

	UPROPERTY()
	float bonusSpeed = 1;

	UPROPERTY()
	float bonusDamage = 1;

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

	void ResetSpeed();

	void MultiplyDamage(float value);

	void ResetDamage();

	UFUNCTION(Reliable, Client)
	void Client_SetHealth();
	void Client_SetHealth_Implementation();

	UFUNCTION(Reliable, Client)
	void Client_SetShield();
	void Client_SetShield_Implementation();

	UFUNCTION(Reliable, Client)
	void Client_SetSpeed(float playerSpeedMultiplier);
	void Client_SetSpeed_Implementation(float playerSpeedMultiplier);

	UFUNCTION(Reliable, Client)
	void Client_ResetSpeed();
	void Client_ResetSpeed_Implementation();

	UFUNCTION(Reliable, Client)
	void Client_SetDamage(float playerDamageMultiplier);
	void Client_SetDamage_Implementation(float playerDamageMultiplier);

	UFUNCTION(Reliable, Client)
	void Client_ResetDamage();
	void Client_ResetDamage_Implementation();

	UPROPERTY(EditAnywhere)
	float maxHealth = 100.0f;

	UPROPERTY(EditAnywhere)
	float maxShield = 100.0f;

	UPROPERTY(replicated)
	float currentHealth;
	UPROPERTY(replicated)
	float currentShield;

	UPROPERTY()
	class AGamePlayerController* GamePlayerController;

	AGamePlayerController* PC;
};