// Fill out your copyright notice in the Description page of Project Settings.


#include "Penguin.h"

APenguin::APenguin()
{
    // Create a first person camera component.
    CheeseBlock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheeseBlock"));
    check(CheeseBlock != nullptr);
    // Attach the camera component to our capsule component.
    CheeseBlock->SetupAttachment(FPSCameraComponent);
}

void APenguin::BeginPlay()
{
    Super::BeginPlay();
    bCanShoot = true;
}

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


        // Transform MuzzleOffset from camera space to world space.
        FVector MuzzleLocation = CheeseBlock->GetComponentLocation();

        // Skew the aim to be slightly upwards.
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 2.0f;
        MuzzleRotation.Yaw += -2.0f;

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
    if (bCanShoot)
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

                if (CheeseBlock)
                {
                    //Set Cheese Block as invisible
                    CheeseBlock->SetVisibility(false);

                    //Activate timer
                    GetWorld()->GetTimerManager().SetTimer(ShootTimer, this, &APenguin::ReactivateShooting, ShootStall, false);
                    bCanShoot = false;
                }
                // Spawn the projectile at the muzzle.
                AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
                if (Projectile)
                {
                    // Set the projectile's initial trajectory.
                    Projectile->damage *= DamageBoostMultiplier;
                    Projectile->Mesh->SetRelativeRotation(CheeseBlock->GetRelativeRotation());
                    FVector LaunchDirection = MuzzleRotation.Vector();
                    Projectile->FireInDirection(LaunchDirection);
                }
            }
        }
    }
}

void APenguin::ReactivateShooting()
{
    CheeseBlock->SetVisibility(true);

    bCanShoot = true;
}