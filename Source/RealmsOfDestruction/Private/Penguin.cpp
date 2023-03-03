// Fill out your copyright notice in the Description page of Project Settings.


#include "Penguin.h"

void APenguin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APenguin::Fire);
}


void APenguin::Fire()
{
    if (ProjectileClass)
    {
        // Attempt to fire a projectile.

        // Get the camera transform.
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // Set MuzzleOffset to spawn projectiles slightly in front of the camera.
        //MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

        // Transform MuzzleOffset from camera space to world space.
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

        // Skew the aim to be slightly upwards.
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 10.0f;

        UWorld* World = GetWorld();
        if (World)
        {
            if (HasAuthority() && IsLocallyControlled())
            {
                //Server Player
                Multicast_Fire(MuzzleLocation, MuzzleRotation);
            }
            else if (!HasAuthority())
            {
                //Client
                Server_Fire(MuzzleLocation, MuzzleRotation);
            }
        }

    }

}

void APenguin::Server_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
{
    Multicast_Fire(MuzzleLocation, MuzzleRotation);
}


void APenguin::Multicast_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
{
    Shoot(MuzzleLocation, MuzzleRotation);
}

void APenguin::Shoot(FVector MuzzleLocation, FRotator MuzzleRotation)
{
    // Attempt to fire a projectile.
    if (ProjectileClass)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            // Spawn the projectile at the muzzle.
            AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                // Set the projectile's initial trajectory.
                Projectile->damage *= bonusDamage;
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->FireInDirection(LaunchDirection);
            }
        }
    }
}