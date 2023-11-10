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
        // Get the camera location and forward vector
        FVector CameraLocation = GetActorLocation() + FPSCameraComponent->GetRelativeLocation();
        FRotator CameraRotation = GetActorRotation() + FPSCameraComponent->GetRelativeRotation();
        //GetActorEyesViewPoint(CameraLocation, CameraRotation);
        FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * MaxTraceDistance); // Adjust MaxTraceDistance to your desired value

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this); // Ignore the actor itself
        // Perform line trace
        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, QueryParams);

        
        // Calculate the vector from muzzle location to hit point
        FVector MuzzleLocation = CheeseBlock->GetComponentLocation();
        FVector ImpactPoint = HitResult.ImpactPoint;
        if (!bHit)
        {
            ImpactPoint = TraceEnd;
        }
        
        FVector MuzzleToImpact = ImpactPoint - MuzzleLocation;

        
        UWorld* World = GetWorld();
        if (World)
        {
            Server_Fire(MuzzleLocation, MuzzleToImpact);
        }
    }

    
}

void APenguin::Server_Fire_Implementation(FVector MuzzleLocation, FVector ShootVector)
{
    Multicast_Fire(MuzzleLocation, ShootVector);
}


void APenguin::Multicast_Fire_Implementation(FVector MuzzleLocation, FVector ShootVector)
{
    Shoot(MuzzleLocation, ShootVector);
}

void APenguin::Shoot(FVector MuzzleLocation, FVector ShootVector)
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
                //Spawn the projectile at the muzzle.
                AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, FRotator::ZeroRotator, SpawnParams);
                if (Projectile)
                {
                    //Set the projectile's initial trajectory.
                    Projectile->damage *= DamageBoostMultiplier;
                    Projectile->Mesh->SetRelativeRotation(CheeseBlock->GetRelativeRotation());
                    
                    //Fire projectile in direction of impact point
                    Projectile->FireInDirection(ShootVector/ShootVector.Length());
                }
            }
        }
    }
}

void APenguin::ReactivateShooting()
{
    if (SpawnProjectileSystem)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SpawnProjectileSystem, CheeseBlock->GetComponentLocation());
    }
    CheeseBlock->SetVisibility(true);
    bCanShoot = true;
}

void APenguin::Ability()
{

}

void APenguin::Ultimate()
{
    if (!bCanShoot)
    {
        return;
    }
    // Get the camera location and forward vector
    FVector CameraLocation = GetActorLocation() + FPSCameraComponent->GetRelativeLocation();
    FRotator CameraRotation = GetActorRotation() + FPSCameraComponent->GetRelativeRotation();
    //GetActorEyesViewPoint(CameraLocation, CameraRotation);
    FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * MaxTraceDistance); // Adjust MaxTraceDistance to your desired value

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // Ignore the actor itself
    // Perform line trace
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, QueryParams);


    // Calculate the vector from muzzle location to hit point
    FVector MuzzleLocation = HitResult.ImpactPoint + FVector(0.0f, 0.0f, 500.0f);
    Server_Ultimate(MuzzleLocation);
}


void APenguin::Server_Ultimate_Implementation(FVector Location)
{



    Multicast_Ultimate(Location);
}


void APenguin::Multicast_Ultimate_Implementation(FVector Location)
{
    // Attempt to fire a projectile.
    if (GiantCheeseClass)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            //Spawn the projectile at the muzzle.
            AProjectile* Projectile = World->SpawnActor<AProjectile>(GiantCheeseClass, Location, FRotator::ZeroRotator, SpawnParams);
            if (Projectile)
            {
                //Set the projectile's initial trajectory.
                Projectile->damage *= DamageBoostMultiplier;

                //Fire projectile in direction of impact point
                Projectile->FireInDirection(FVector(0.0f, 0.0f, -1.0f));
            }
        }
    }
}
