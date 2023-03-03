// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

ABot::ABot()
{
	bReplicates = true;

	MuzzleLocationL = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleL"));
	MuzzleLocationR = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleR"));

    MuzzleLocationL->SetRelativeLocation(FVector(64.0f, -34.46f, 21.3f));
    MuzzleLocationR->SetRelativeLocation(FVector(64.0f, 34.46f, 21.3f));


	MuzzleLocationL->SetupAttachment(GetMesh());
	MuzzleLocationR->SetupAttachment(GetMesh());

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

}


void ABot::BeginPlay()
{
	Super::BeginPlay();

	MuzzleLocationL->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Bone_001_L"));
	MuzzleLocationR->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Bone_001_R"));
}

void ABot::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABot, FlySpeed);
	DOREPLIFETIME(ABot, isShooting);
}

void ABot::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABot::Fall);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABot::Fire);

}

// Called every frame
void ABot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Apply flying force
}

// Fly upwards
void ABot::StartJump()
{
	AddMovementInput(FVector(0, 0, 1), 1000.0f);
}

// Fly downwards
void ABot::StopJump()
{

}

void ABot::StartSprint()
{
	GetCharacterMovement()->MaxFlySpeed *= speedMultiplier;
	Server_StartSprint();
}

void ABot::StopSprint()
{
	GetCharacterMovement()->MaxFlySpeed /= speedMultiplier;
	Server_StopSprint();
}

void ABot::Server_StartSprint_Implementation()
{
	GetCharacterMovement()->MaxFlySpeed *= speedMultiplier;
}

void ABot::Server_StopSprint_Implementation()
{
	GetCharacterMovement()->MaxFlySpeed /= speedMultiplier;
}

void ABot::Fall()
{
	AddMovementInput(FVector(0, 0, 1), -600.0f);
}

void ABot::Fire()
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



        // Skew the aim to be slightly upwards.
        FRotator MuzzleRotation = CameraRotation;

        MuzzleRotation.Pitch += 10.0f;

        UWorld* World = GetWorld();
        if (World)
        {
            if (HasAuthority() && IsLocallyControlled())
            {
                //Server Player
                Multicast_Fire(MuzzleLocationL->GetComponentLocation(), MuzzleRotation);
                Multicast_Fire(MuzzleLocationR->GetComponentLocation(), MuzzleRotation);

            }
            else if (!HasAuthority())
            {
                Shoot(MuzzleLocationL->GetComponentLocation(), MuzzleRotation);
                //Client
                Server_Fire(MuzzleLocationL->GetComponentLocation(), MuzzleRotation);

                Shoot(MuzzleLocationR->GetComponentLocation(), MuzzleRotation);
                //Client
                Server_Fire(MuzzleLocationR->GetComponentLocation(), MuzzleRotation);
            }
        }

    }
}


void ABot::Server_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
{
    Shoot(MuzzleLocation, MuzzleRotation);
}


void ABot::Multicast_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
{
    Shoot(MuzzleLocation, MuzzleRotation);
}

void ABot::Shoot(FVector MuzzleLocation, FRotator MuzzleRotation)
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