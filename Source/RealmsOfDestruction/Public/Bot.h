// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterMovement.h"
#include "Projectile.h"
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

	virtual void BeginPlay() override;

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

	UPROPERTY(EditDefaultsOnly)
	float fireRate = 0.15f;
	
	UPROPERTY(Replicated)
	bool isShooting;

	UFUNCTION()
	void Fire();

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* MuzzleLocationL;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* MuzzleLocationR;

	// Sets sprint flag when key is pressed.
	virtual void Server_StartSprint_Implementation() override;


	// Clears sprint flag when key is released.
	virtual void Server_StopSprint_Implementation() override;

	UFUNCTION()
	void Fall();

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

	UFUNCTION(Reliable, Server)
		void Server_Fire(FVector MuzzleLocation, FRotator MuzzleRotation);
	void Server_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);

	UFUNCTION(Reliable, NetMulticast)
		void Multicast_Fire(FVector MuzzleLocation, FRotator MuzzleRotation);
	void Multicast_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);

	void Shoot(FVector MuzzleLocation, FRotator MuzzleRotation);

private:
	UPROPERTY(Replicated)
		float FlySpeed;
};
