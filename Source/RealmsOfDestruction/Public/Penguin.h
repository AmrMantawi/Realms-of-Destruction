// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterMovement.h"
#include "Projectile.h"
#include "Penguin.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API APenguin : public ACharacterMovement
{
	GENERATED_BODY()
	

	// Function that handles firing projectiles.
	UFUNCTION()
	void Fire();

	UFUNCTION(Reliable, Server)
	void Server_Fire(FVector MuzzleLocation, FRotator MuzzleRotation);
	void Server_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);

	UFUNCTION(Reliable, NetMulticast)
	void Multicast_Fire(FVector MuzzleLocation, FRotator MuzzleRotation);
	void Multicast_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);

	void Shoot(FVector MuzzleLocation, FRotator MuzzleRotation);

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere)
	FVector MuzzleOffset = FVector(40.f, 0.f, -50.f);

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

};
