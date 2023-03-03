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

	AGreenBean();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// Function that handles firing projectiles.
	UFUNCTION()
	void Fire();

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleLocation1;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleLocation2;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleLocation3;
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
	
};
