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
	void SetCharacterState(ECharacterState State);

	UFUNCTION()
	ECharacterState GetCharacterState();
protected:
	UFUNCTION()
	void Setup();

	UFUNCTION(Client, Reliable)
	void Client_Setup();
	void Client_Setup_Implementation();

	UFUNCTION(Client, Reliable)
	void Client_UnSetup();
	void Client_UnSetup_Implementation();

	UFUNCTION()
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION()
	virtual void UnPossessed() override;

private:
	UPROPERTY()
	class UHealthBar* PlayerHealthBar;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHealthBar> HealthBarClass;
	UPROPERTY()
	bool bPauseMenuDisplayed = false;

	UPROPERTY()
	bool bCharacterSelectionMenuDisplayed = false;

protected:
	UFUNCTION()
	void MenuMode();

	UFUNCTION()
	void PlayMode();

	UFUNCTION()
	void OnRep_HandleCharacterState();

private:
	
	UPROPERTY(ReplicatedUsing = OnRep_HandleCharacterState)
	ECharacterState CharacterState;

	UFUNCTION()
	void OutlineCharacter();

	UFUNCTION()
	void Die();

	UFUNCTION()
	void DeadState();

	FTimerHandle TimerHandle;

protected:

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "PostProcessing")
	class UPostProcessComponent* PostProcessComponent;

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


	UPROPERTY(EditDefaultsOnly, Category = "Character Movement: Walking")
	float SprintSpeedMultiplier = 2;

	UPROPERTY()
	float SpeedBoostMultiplier = 1;

	UPROPERTY()
	float DamageBoostMultiplier = 1;

	public:

	//Take Damage
	void CharacterTakeDamage(float Value);

	//Damage To Health
	void DamageHealth(float Value);
	
	//Damage to Sheild
	void DamageShield(float Value);

	//Gain Health
	void GainHealth(float Value);

	//Gain Sheild
	void GainShield(float Value);

private:
	void ResetSpeed();
	void ResetDamage();

public:
	void MultiplySpeed(float Value);
	void MultiplyDamage(float Value);

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

private:
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxShield = 100.0f;

	UPROPERTY(replicated)
	float CurrentHealth;
	UPROPERTY(replicated)
	float CurrentShield;
};