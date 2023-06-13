// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterMovement.h"
#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Penguin.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API APenguin : public ACharacterMovement
{
	GENERATED_BODY()
	
public:
	APenguin();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Function that handles firing projectiles.
	UFUNCTION()
	void Fire();

	// Sends fire signal to server.
	UFUNCTION(Reliable, Server)
	void Server_Fire(FVector MuzzleLocation, FRotator MuzzleRotation);
	void Server_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);

	UFUNCTION(Reliable, NetMulticast)
	void Multicast_Fire(FVector MuzzleLocation, FRotator MuzzleRotation);
	void Multicast_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);

	//Shoot Projectile at specified location and rotation
	void Shoot(FVector MuzzleLocation, FRotator MuzzleRotation);

	//Setup player input
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//Mesh of projectile
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* CheeseBlock;

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere)
	FVector MuzzleOffset = FVector(40.f, 0.f, -50.f);

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	//Stall time in between shots
	UPROPERTY(EditAnywhere)
	float ShootStall = 1.0f;

	UPROPERTY()
	bool bCanShoot;

	UFUNCTION()
	void ReactivateShooting();

	UPROPERTY()
	FTimerHandle ShootTimer;
};
