// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterMovement.h"
#include "Projectile.h"
#include "GreenBean.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API AGreenBean : public ACharacterMovement
{
	GENERATED_BODY()


public:
	// Function that handles firing projectiles.
	UFUNCTION()
	void Fire();

	UFUNCTION(Reliable, Server)
	void Server_Fire();
	void Server_Fire_Implementation();

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere)
	FVector MuzzleOffset = FVector(40.f, 0.f, -50.f);

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;
	
};
