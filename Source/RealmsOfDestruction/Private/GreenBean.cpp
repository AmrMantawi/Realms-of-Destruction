// Fill out your copyright notice in the Description page of Project Settings.


#include "GreenBean.h"

AGreenBean::AGreenBean()
{
    bReplicates = true;

    MuzzleLocation1 = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle1"));
    MuzzleLocation2 = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle2"));
    MuzzleLocation3 = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle3"));


    MuzzleLocation1->SetRelativeLocation(FVector(6.572663f, 0.0f, 49.726662f));
    MuzzleLocation2->SetRelativeLocation(FVector(-3.282223f, 0.0f, 2.269668f));
    MuzzleLocation3->SetRelativeLocation(FVector(4.912164f, 34.46f, -44.918054f));

    MuzzleLocation1->SetupAttachment(GetMesh());
    MuzzleLocation2->SetupAttachment(GetMesh());
    MuzzleLocation3->SetupAttachment(GetMesh());
}

void AGreenBean::BeginPlay()
{
    Super::BeginPlay();

    MuzzleLocation1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Bone_008"));
    MuzzleLocation2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Bone_006"));
    MuzzleLocation3->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("Bone"));

}

void AGreenBean::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGreenBean::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGreenBean::Fire);
}


void AGreenBean::Fire()
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
                Multicast_Fire(MuzzleLocation1->GetComponentLocation(), MuzzleRotation);
                Multicast_Fire(MuzzleLocation2->GetComponentLocation(), MuzzleRotation);
                Multicast_Fire(MuzzleLocation3->GetComponentLocation(), MuzzleRotation);


            }
            else if (!HasAuthority())
            {
                Shoot(MuzzleLocation1->GetComponentLocation(), MuzzleRotation);
                //Client
                Server_Fire(MuzzleLocation1->GetComponentLocation(), MuzzleRotation);

                Shoot(MuzzleLocation2->GetComponentLocation(), MuzzleRotation);
                //Client
                Server_Fire(MuzzleLocation2->GetComponentLocation(), MuzzleRotation);
                Shoot(MuzzleLocation3->GetComponentLocation(), MuzzleRotation);
                //Client
                Server_Fire(MuzzleLocation3->GetComponentLocation(), MuzzleRotation);
            }
        }
    }
}

    void AGreenBean::Server_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
    {
        Shoot(MuzzleLocation, MuzzleRotation);
    }

    void AGreenBean::Multicast_Fire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
    {
        Shoot(MuzzleLocation, MuzzleRotation);
    }

    void AGreenBean::Shoot(FVector MuzzleLocation, FRotator MuzzleRotation)
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