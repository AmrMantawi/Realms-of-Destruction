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
	void Server_Fire(FVector MuzzleLocation, FVector ShootVector);
	void Server_Fire_Implementation(FVector MuzzleLocation, FVector ShootVector);

	UFUNCTION(Reliable, NetMulticast)
	void Multicast_Fire(FVector MuzzleLocation, FVector ShootVector);
	void Multicast_Fire_Implementation(FVector MuzzleLocation, FVector ShootVector);

	//Shoot Projectile at specified location and rotation
	void Shoot(FVector MuzzleLocation, FVector ShootVector);

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
	
	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> GiantCheeseClass;

	//Stall time in between shots
	UPROPERTY(EditAnywhere)
	float ShootStall = 1.0f;

	UPROPERTY()
	bool bCanShoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UNiagaraSystem* SpawnProjectileSystem;

	UFUNCTION()
	void ReactivateShooting();

	UPROPERTY()
	FTimerHandle ShootTimer;

	UFUNCTION(Reliable, Server)
	void Server_Ultimate(FVector Location);
	void Server_Ultimate_Implementation(FVector Location);

	UFUNCTION(Reliable, NetMulticast)
	void Multicast_Ultimate(FVector Location);
	void Multicast_Ultimate_Implementation(FVector Location);

	void Ultimate() override;
protected:
	UPROPERTY(EditDefaultsOnly)
	float MaxTraceDistance = 10000.0f;

	void Ability() override;


};
