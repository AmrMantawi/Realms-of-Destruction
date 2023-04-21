// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMovement.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.h"
#include "GamePlayerController.h"
#include "CharacterSelectionMenu.h"
#include "PlayesrDisplay.h"
#include "SettingsMenu.h"
#include "PauseMenu.h"
#include "RealmsPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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
    bReplicates = true;

    characterState = ECharacterState::Normal;
}

void ACharacterMovement::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACharacterMovement, currentHealth);
    DOREPLIFETIME(ACharacterMovement, currentShield);
    DOREPLIFETIME(ACharacterMovement, characterState);


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

void ACharacterMovement::UnPossessed()
{
    Client_UnSetup_Implementation();
    Super::UnPossessed();
}


void ACharacterMovement::Setup()
{
        Client_Setup();

        currentHealth = maxHealth;
        currentShield = maxShield;
}

void ACharacterMovement::Client_UnSetup_Implementation()
{
    if (IsLocallyControlled())
    {
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
        if (playerSettingsMenu)
        {
            playerSettingsMenu->RemoveFromParent();
            playerSettingsMenu = nullptr;
        }
        if (playerCharacterSelection)
        {
            playerCharacterSelection->RemoveFromParent();
            playerCharacterSelection = nullptr;
        }
        if (playersMenu)
        {
            playersMenu->RemoveFromParent();
            playersMenu = nullptr;
        }
    }

}

void ACharacterMovement::Client_Setup_Implementation()
{
    if (AGamePlayerController* PC = GetController<AGamePlayerController>())
    {
        if (IsLocallyControlled() && HealthBarClass)
        {
            if ((PlayerHealthBar = CreateWidget<UHealthBar>(PC, HealthBarClass)) != nullptr) {
                PlayerHealthBar->AddToPlayerScreen();
            }

            //Set UI values For Health and Shield
            PlayerHealthBar->SetHealth(1);
            PlayerHealthBar->SetShield(1);
        }

        // UE_LOG(LogTemp, Warning, TEXT("Client Menu Setup"));

        if (characterSelectionClass)
        {
            // UE_LOG(LogTemp, Warning, TEXT("Creating Selection Menu"));
            playerCharacterSelection = CreateWidget<UCharacterSelectionMenu>(PC, characterSelectionClass);
        }

        if (PauseMenuClass)
        {
            if (SettingsMenuClass)
            {
                //Create Pause Menu
                playerSettingsMenu = CreateWidget<USettingsMenu>(PC, SettingsMenuClass);
            }
            playerPauseMenu = CreateWidget<UPauseMenu>(PC, PauseMenuClass);
            playerPauseMenu->Resume->OnClicked.AddDynamic(this, &ACharacterMovement::TogglePauseMenu);
            playerPauseMenu->Settings->OnClicked.AddDynamic(this, &ACharacterMovement::ToggleSettingsMenu);
            playerPauseMenu->ChangeCharacter->OnClicked.AddDynamic(this, &ACharacterMovement::ToggleCharacterSelectionMenu);

        }
        if (PlayersMenuClass)
        {
            //UE_LOG(LogTemp, Warning, TEXT("Creating Selection Menu"));
            playersMenu = CreateWidget<UPlayesrDisplay>(PC, PlayersMenuClass);
        }
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
    if (playerPauseMenu)
    {
        if (!pauseMenuDisplayed)
        {
            //Display Pause Menu
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

            MenuMode();
        }
        else
        {
            //Hide Pause Menu
            if (settingsDisplayed)
            {
                ToggleSettingsMenu();
            }
            else if (characterSelectionMenuDisplayed)
            {
                ToggleCharacterSelectionMenu();
            }
            else {
                playerPauseMenu->RemoveFromParent();
                pauseMenuDisplayed = false;
                PlayMode();
            }
        }
    }
}
void ACharacterMovement::ToggleSettingsMenu()
{
    if (playerSettingsMenu)
    {
        if (!settingsDisplayed)
        {
            playerSettingsMenu->AddToPlayerScreen();
            settingsDisplayed = true;
        }
        else
        {
            playerSettingsMenu->RemoveFromParent();
            settingsDisplayed = false;
        }
    }
}

void ACharacterMovement::TogglePlayersMenu()
{
    //UE_LOG(LogTemp, Warning, TEXT("Player button pressed"));

    if (playersMenu)
    {
        //UE_LOG(LogTemp, Warning, TEXT("players menu"));

        if (!playersMenuDisplayed)
        {
            playersMenu->AddToPlayerScreen();
            playersMenuDisplayed = true;
           // UE_LOG(LogTemp, Warning, TEXT("Display now..."));
        }
        else
        {
            playersMenu->RemoveFromParent();
            playersMenuDisplayed = false;
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
    if (playerSettingsMenu)
    {
        playerSettingsMenu->RemoveFromParent();
        playerSettingsMenu = nullptr;
    }
    if (playerCharacterSelection)
    {
        playerCharacterSelection->RemoveFromParent();
        playerCharacterSelection = nullptr;
    }
    if (playersMenu)
    {
        playersMenu->RemoveFromParent();
        playersMenu = nullptr;
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

    PlayerInputComponent->BindAction("Player", IE_Pressed, this, &ACharacterMovement::TogglePlayersMenu);
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
    Server_StartSprint();
}

void ACharacterMovement::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed /= speedMultiplier;
    Server_StopSprint();
}

void ACharacterMovement::Server_StartSprint_Implementation()
{
    GetCharacterMovement()->MaxWalkSpeed *= speedMultiplier;
}

void ACharacterMovement::Server_StopSprint_Implementation()
{
    GetCharacterMovement()->MaxWalkSpeed /= speedMultiplier;
}

void ACharacterMovement::CharacterTakeDamage(float value)
{
    //UE_LOG(LogTemp, Warning, TEXT("Taking Damage"));
    //Damage Sheild First
    //if (HasAuthority())
    {
        DamageShield(value);
    }
}

void ACharacterMovement::DamageShield(float value)
{
    float difference = (currentShield - value);
    
    //Sheild is still left
    if (difference > 0)
    { 
        currentShield = difference;
    }
    else
    {
        //Damage health with remaining damage
        currentShield = 0;
        if (difference < 0)
        {
            DamageHealth((difference * -1));
        }
    }
    Client_SetShield();
}

void ACharacterMovement::DamageHealth(float value)
{
    float difference = (currentHealth - value);
    //Health iis still left
    if (difference > 0)
    {
        currentHealth = difference;
    }
    else
    {
        //Character dies
        currentHealth = 0;
        if (difference < 0)
        {
            Die();
        }
    }
    Client_SetHealth();
}

void ACharacterMovement::GainHealth(float value)
{
    float Addition = (currentHealth + value);
    //Maximum health is reached
    if (Addition >= maxHealth)
    {
        currentHealth = maxHealth;
    }
    //Maximum Health is not reached
    else
    {
        currentHealth = Addition;
    }
    Client_SetHealth();
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
    Client_SetShield();
}
//

void ACharacterMovement::MultiplyDamage(float value)
{
    if (HasAuthority())
    {
        //Server
        bonusDamage *= value;
        //UE_LOG(LogTemp, Warning, TEXT("Server Damage"));

        if (!IsLocallyControlled())
        {
            //Client
            //UE_LOG(LogTemp, Warning, TEXT("Client Damage"));
            Client_SetDamage(value);
        }
        GetWorldTimerManager().SetTimer(timerHandle, this, &ACharacterMovement::ResetDamage, 1.0f, false, 3.0f);
    }
}

void ACharacterMovement::ResetDamage()
{
    if (HasAuthority())
    {
        //Server
        bonusDamage = 1;
        if (!IsLocallyControlled())
        {
            //Client
            Client_ResetDamage();

        }
    }
}

void ACharacterMovement::Client_SetDamage_Implementation(float playerDamageMultiplier = 1)
{
    if (IsLocallyControlled())
    {
        bonusDamage = playerDamageMultiplier;
    }
}

void ACharacterMovement::Client_ResetDamage_Implementation()
{
    if (IsLocallyControlled())
    {
        GetCharacterMovement()->MaxWalkSpeed /= bonusDamage;
    }
}


//
void ACharacterMovement::MultiplySpeed(float value)
{
    if (HasAuthority())
    {
        //Server
        GetCharacterMovement()->MaxWalkSpeed /= bonusSpeed;
        bonusSpeed *= value;
        GetCharacterMovement()->MaxWalkSpeed *= bonusSpeed;
        FPSCameraComponent->SetFieldOfView(100);
        //UE_LOG(LogTemp, Warning, TEXT("Server Speed"));

        if (!IsLocallyControlled())
        {
            //Client
            //UE_LOG(LogTemp, Warning, TEXT("Client Speed"));
            Client_SetSpeed(value);
        }
        GetWorldTimerManager().SetTimer(timerHandle, this, &ACharacterMovement::ResetSpeed, 1.0f, false, 3.0f);
    }    
}

void ACharacterMovement::ResetSpeed()
{
    if (HasAuthority())
    {
        //Server
        GetCharacterMovement()->MaxWalkSpeed /= bonusSpeed;
        bonusSpeed = 1;
        FPSCameraComponent->SetFieldOfView(90);
        if (!IsLocallyControlled())
        {
            //Client
            Client_ResetSpeed();

        }
    }
}

void ACharacterMovement::Client_SetSpeed_Implementation(float playerSpeedMultiplier = 1)
{
    if (IsLocallyControlled())
    {
        bonusSpeed = playerSpeedMultiplier;
        GetCharacterMovement()->MaxWalkSpeed *= bonusSpeed;
        FPSCameraComponent->SetFieldOfView(100);
    }
}

void ACharacterMovement::Client_ResetSpeed_Implementation()
{
    if (IsLocallyControlled())
    {
        GetCharacterMovement()->MaxWalkSpeed /= bonusSpeed;
        FPSCameraComponent->SetFieldOfView(90);
    }
}




void ACharacterMovement::Client_SetHealth_Implementation()
{
    if (IsLocallyControlled() && PlayerHealthBar)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Health Set: %f"), currentHealth);
        PlayerHealthBar->SetHealth((currentHealth > 0) ? currentHealth / maxHealth : 0);
    }
}
void ACharacterMovement::Client_SetShield_Implementation()
{
    if (IsLocallyControlled() && PlayerHealthBar)
    {
       // UE_LOG(LogTemp, Warning, TEXT("Shield Set: %f"), currentShield);
        PlayerHealthBar->SetShield((currentShield > 0) ? currentShield / maxShield : 0);
    }
}


void ACharacterMovement::MenuMode()
{
    if (AGamePlayerController* PC = GetController<AGamePlayerController>())
    {
        //UE_LOG(LogTemp, Warning, TEXT("Menu Mode"));

        FInputModeGameAndUI GameAndUI;

        PC->SetInputMode(GameAndUI);
        PC->SetShowMouseCursor(true);
    }
}

void ACharacterMovement::PlayMode()
{
    if(AGamePlayerController* PC = GetController<AGamePlayerController>())
    {
        //UE_LOG(LogTemp, Warning, TEXT("Play Mode"));

        FInputModeGameOnly GameOnly;

        PC->SetInputMode(GameOnly);
        PC->SetShowMouseCursor(false);
    }

}



//Character State getter and setter
void ACharacterMovement::setCharacterState(ECharacterState state)
{
    UE_LOG(LogTemp, Warning, TEXT("Setting Character State"));
    characterState = state;
    OnRep_HandleCharacterState();
}

ECharacterState ACharacterMovement::getCharacterState()
{
    return characterState;
}

//Handle Each Character State
void ACharacterMovement::OnRep_HandleCharacterState()
{
    if (characterState == ECharacterState::Dead)
    {
        DeadState();
    }
}


void ACharacterMovement::Die()
{
    //UE_LOG(LogTemp, Warning, TEXT("Dead"));
    if (HasAuthority())
    {
        if (ARealmsPlayerState* state = this->GetPlayerState<ARealmsPlayerState>())
        {
            state->AddDeath();
        }
        if (AGamePlayerController* PC = GetController<AGamePlayerController>())
        {
            PC->Die();
        }
    }
}

void ACharacterMovement::DeadState()
{
    UE_LOG(LogTemp, Warning, TEXT("Dead State"));

    this->GetMesh()->SetSimulatePhysics(true);
    this->GetMesh()->SetCollisionProfileName(TEXT("PhysicsActor"));
    this->GetMesh()->SetOwnerNoSee(false);

    if (HasAuthority())
    {
        this->SetLifeSpan(5);
    }
}
