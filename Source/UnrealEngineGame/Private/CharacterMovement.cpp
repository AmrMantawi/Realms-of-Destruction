// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMovement.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.h"
#include "GamePlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACharacterMovement::ACharacterMovement()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create a first person camera component.
    FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    check(FPSCameraComponent != nullptr);

    // Attach the camera component to our capsule component.
    FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

    // Position the camera slightly above the eyes.
    FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

    // Enable the pawn to control camera rotation.
    FPSCameraComponent->bUsePawnControlRotation = true;



    UE_LOG(LogTemp, Warning, TEXT("Created---"));
}

// Called when the game starts or when spawned
void ACharacterMovement::BeginPlay()
{
	Super::BeginPlay();

    if (HealthBarClass)
    {
        /*
        UE_LOG(LogTemp, Warning, TEXT("Adding to player screen"));
        if (GetController<AGamePlayerController>() == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("its null"));
        }
        //AGamePlayerController* PC = GetController<AGamePlayerController>();
       check(PC);
        PlayerHealthBar = CreateWidget<UHealthBar>(PC, HealthBarClass);
        check(PlayerHealthBar);
        PlayerHealthBar->AddToPlayerScreen();

        UE_LOG(LogTemp, Warning, TEXT("Added to player screen"));*/
    }
}

void ACharacterMovement::Setup()
{
    if (IsLocallyControlled())
    {
        UE_LOG(LogTemp, Warning, TEXT("Is Locally Controlled"));
    }

    if (HealthBarClass)
    {
        PC = GetController<AGamePlayerController>();
        check(PC);
        PlayerHealthBar = CreateWidget<UHealthBar>(PC, HealthBarClass);
        check(PlayerHealthBar);
        PlayerHealthBar->AddToPlayerScreen();

        //Set Local Values For Health and Shield
        currentHealth = maxHealth;
        currentShield = maxShield;

        //Set UI values For Health and Shield
        PlayerHealthBar->SetHealth(1);
        PlayerHealthBar->SetShield(1);

        UE_LOG(LogTemp, Warning, TEXT("Added to player screen"));
    }

}

void ACharacterMovement::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{

    Super::EndPlay(EndPlayReason);
    if (PlayerHealthBar)
    {
        PlayerHealthBar->RemoveFromParent();
        PlayerHealthBar = nullptr;
    }
}

ACharacterMovement::~ACharacterMovement()
{

}


// Called every frame
void ACharacterMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up "movement" bindings.
    PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterMovement::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterMovement::MoveRight);

    // Set up "look" bindings.
    PlayerInputComponent->BindAxis("Turn", this, &ACharacterMovement::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &ACharacterMovement::AddControllerPitchInput);

    // Set up "action" bindings.
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterMovement::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacterMovement::StopJump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACharacterMovement::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACharacterMovement::StopSprint);

    // Set up "fire" brindings
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterMovement::Fire);

}

void ACharacterMovement::MoveForward(float Value)
{
    // Find out which way is "forward" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void ACharacterMovement::MoveRight(float Value)
{
    // Find out which way is "right" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void ACharacterMovement::StartJump()
{
    bPressedJump = true;
}

void ACharacterMovement::StopJump()
{
    bPressedJump = false;
}

void ACharacterMovement::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed *= speedMultiplier;
}

void ACharacterMovement::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed /= speedMultiplier;
}

void ACharacterMovement::Fire()
{

    // Attempt to fire a projectile.
    if (ProjectileClass)
    {
        // Get the camera transform.
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // Set MuzzleOffset to spawn projectiles slightly in front of the camera.
        MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

        // Transform MuzzleOffset from camera space to world space.
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

        // Skew the aim to be slightly upwards.
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 10.0f;

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
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->FireInDirection(LaunchDirection);
            }
        }
    }
}

void ACharacterMovement::CharacterTakeDamage(float value)
{
    DamageShield(value);
}

void ACharacterMovement::DamageShield(float value)
{
    float Difference = (currentShield - value);
    if (Difference >= 0)
    {
        PlayerHealthBar->SetShield(Difference / maxShield);
    }
    else
    {
        PlayerHealthBar->SetShield(0);
        DamageHealth((Difference * -1));
    }
}

void ACharacterMovement::DamageHealth(float value)
{
    float Difference = (currentHealth - value);
    if (Difference >= 0)
    {
        PlayerHealthBar->SetHealth(Difference / maxHealth);
    }
    else
    {
        PlayerHealthBar->SetHealth(0);
        Die();
    }
}

void ACharacterMovement::GainHealth(float value)
{
    float Addition = (currentHealth + value);
    if (Addition >= maxHealth)
    {
        PlayerHealthBar->SetHealth(1);
    }
    else
    {
        PlayerHealthBar->SetHealth(Addition / maxHealth);
    }
}

void ACharacterMovement::GainShield(float value)
{
    float Addition = (currentShield + value);
    if (Addition >= maxShield)
    {
        PlayerHealthBar->SetShield(1);
    }
    else
    {
        PlayerHealthBar->SetShield(Addition / maxHealth);
    }
}

void ACharacterMovement::Die()
{
    PC->Die();
}
