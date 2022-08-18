// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMovement.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.h"
#include "GamePlayerController.h"
#include "CharacterSelectionMenu.h"
#include "PauseMenu.h"
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

    currentHealth = maxHealth;
    currentShield = maxShield;

    UE_LOG(LogTemp, Warning, TEXT("currentHealth: %d"), currentHealth);
    UE_LOG(LogTemp, Warning, TEXT("currentShield: %d"), currentShield);
    UE_LOG(LogTemp, Warning, TEXT("maxHealth: %d"), maxHealth);
    UE_LOG(LogTemp, Warning, TEXT("maxShield: %d"), maxShield);


    UE_LOG(LogTemp, Warning, TEXT("Created---"));
}

// Called when the game starts or when spawned
void ACharacterMovement::BeginPlay()
{
	Super::BeginPlay();
}
void ACharacterMovement::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    Setup();
}

void ACharacterMovement::Setup()
{
    //if (IsLocallyControlled())
    //{
    //    if (HealthBarClass)
    //    {
    //        PC = GetController<AGamePlayerController>();
    //        check(PC);
    //        PlayerHealthBar = CreateWidget<UHealthBar>(PC, HealthBarClass);
    //        check(PlayerHealthBar);
    //        PlayerHealthBar->AddToPlayerScreen();

    //        //Set UI values For Health and Shield
    //        PlayerHealthBar->SetHealth(1);
    //        PlayerHealthBar->SetShield(1);
    //    }
    //}
    //else
    //{
        Client_Setup();
    //}
}

void ACharacterMovement::Client_Setup_Implementation()
{
    if (IsLocallyControlled() && HealthBarClass)
    {
        PC = GetController<AGamePlayerController>();
        check(PC);
        PlayerHealthBar = CreateWidget<UHealthBar>(PC, HealthBarClass);
        check(PlayerHealthBar);
        PlayerHealthBar->AddToPlayerScreen();

        //Set UI values For Health and Shield
        PlayerHealthBar->SetHealth(1);
        PlayerHealthBar->SetShield(1);
    }

    UE_LOG(LogTemp, Warning, TEXT("Client Menu Setup"));

    if (characterSelectionClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Creating Selection Menu"));
        playerCharacterSelection = CreateWidget<UCharacterSelectionMenu>(PC, characterSelectionClass);
    }

    if (PauseMenuClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Creating Pause Menu"));
        playerPauseMenu = CreateWidget<UPauseMenu>(PC, PauseMenuClass);
        playerPauseMenu->Resume->OnClicked.AddDynamic(this, &ACharacterMovement::TogglePauseMenu);
        playerPauseMenu->ChangeCharacter->OnClicked.AddDynamic(this, &ACharacterMovement::ToggleCharacterSelectionMenu);
    }
}

void ACharacterMovement::ToggleCharacterSelectionMenu()
{
    if (playerCharacterSelection)
    {
        if (!characterSelectionMenuDisplayed)
        {
            playerCharacterSelection->AddToPlayerScreen();
            characterSelectionMenuDisplayed = true;
        }
        else
        {
            playerCharacterSelection->RemoveFromParent();
            characterSelectionMenuDisplayed = false;
        }
    }
}

void ACharacterMovement::TogglePauseMenu()
{
    UE_LOG(LogTemp, Warning, TEXT("Toggle Pause Menu"));

    if (playerPauseMenu)
    {
        UE_LOG(LogTemp, Warning, TEXT("Pause Menu Exists"));

        if (!pauseMenuDisplayed)
        {
            if (characterSelectionMenuDisplayed)
            {
                playerPauseMenu->ChangeCharacter->SetVisibility(ESlateVisibility::Hidden);
            }
            else
            {
                playerPauseMenu->ChangeCharacter->SetVisibility(ESlateVisibility::Visible);
            }
            playerPauseMenu->AddToPlayerScreen();
            pauseMenuDisplayed = true;

            UE_LOG(LogTemp, Warning, TEXT("Toggle Pause Menu: On"));

            MenuMode();
        }
        else
        {
            playerPauseMenu->RemoveFromParent();
            pauseMenuDisplayed = false;
            UE_LOG(LogTemp, Warning, TEXT("Toggle Pause Menu: Off"));

            PlayMode();
        }
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
    if (playerPauseMenu)
    {
        playerPauseMenu->RemoveFromParent();
        playerPauseMenu = nullptr;
    }
    if (playerCharacterSelection)
    {
        playerCharacterSelection->RemoveFromParent();
        playerCharacterSelection = nullptr;
    }
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

    PlayerInputComponent->BindAction("Pause Game", IE_Pressed, this, &ACharacterMovement::TogglePauseMenu);
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

void ACharacterMovement::CharacterTakeDamage(float value)
{
    UE_LOG(LogTemp, Warning, TEXT("Taking Damage"));
    if (HasAuthority())
    {
        DamageShield(value);
    }
}

void ACharacterMovement::DamageShield(float value)
{
    float Difference = (currentShield - value);
    if (Difference >= 0)
    {
        currentShield = Difference;
    }
    else
    {
        currentShield = 0;
        DamageHealth((Difference * -1));
    }

    Client_SetShield(currentShield);
}

void ACharacterMovement::DamageHealth(float value)
{
    float Difference = (currentHealth - value);
    if (Difference >= 0)
    {
        currentHealth = Difference;
    }
    else
    {
        currentHealth = 0;
        if(HasAuthority())
        {
            Die();
        }
    }
    Client_SetHealth(currentHealth);
}

void ACharacterMovement::GainHealth(float value)
{
    float Addition = (currentHealth + value);
    if (Addition >= maxHealth)
    {
        currentHealth = maxHealth;
    }
    else
    {
        currentHealth = Addition;
    }
    Client_SetHealth(currentHealth);
}

void ACharacterMovement::GainShield(float value)
{
    float Addition = (currentShield + value);
    if (Addition >= maxShield)
    {
        currentShield = maxShield;
    }
    else
    {
        currentShield = Addition;
    }
    Client_SetShield(currentShield);
}


void ACharacterMovement::Client_SetHealth_Implementation(float currentPlayerHealth)
{
    if (IsLocallyControlled() && PlayerHealthBar)
    {
        currentHealth = currentPlayerHealth;
        UE_LOG(LogTemp, Warning, TEXT("Health Taking Damage"));
        UE_LOG(LogTemp, Warning, TEXT("currentHealth: %d"), currentHealth);
        PlayerHealthBar->SetHealth(currentPlayerHealth / maxHealth);
    }
}
void ACharacterMovement::Client_SetShield_Implementation(float currentPlayerShield)
{
    if (IsLocallyControlled() && PlayerHealthBar)
    {
        currentShield = currentPlayerShield;
        UE_LOG(LogTemp, Warning, TEXT("Shield Taking Damage"));
        UE_LOG(LogTemp, Warning, TEXT("currentShield: %d"), currentShield);
        PlayerHealthBar->SetShield(currentPlayerShield / maxShield);
    }

}

void ACharacterMovement::MenuMode()
{
    if (PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Menu Mode"));

        FInputModeGameAndUI GameAndUI;

        PC->SetInputMode(GameAndUI);
        PC->SetShowMouseCursor(true);
    }
}

void ACharacterMovement::PlayMode()
{
    if(PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Play Mode"));

        FInputModeGameOnly GameOnly;

        PC->SetInputMode(GameOnly);
        PC->SetShowMouseCursor(false);
    }

}

void ACharacterMovement::Die()
{
    UE_LOG(LogTemp, Warning, TEXT("Dead"));
    PC->Die();
}
