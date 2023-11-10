// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterMovement.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.h"
#include "GamePlayerController.h"
#include "RealmsPlayerState.h"
#include "Components/PostProcessComponent.h"
#include "NiagaraComponent.h"
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

    //Create post processing component
    PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
    PostProcessComponent->SetupAttachment(GetRootComponent());
    
    //Create niagra lightning effect component
    LightningTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LightningTrail"));
    LightningTrail->SetupAttachment(GetRootComponent());
}

void ACharacterMovement::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACharacterMovement, CurrentHealth);
    DOREPLIFETIME(ACharacterMovement, CurrentShield);
    DOREPLIFETIME(ACharacterMovement, CharacterState);
    DOREPLIFETIME(ACharacterMovement, bLightningEffectActive);
}

// Called when the game starts or when spawned
void ACharacterMovement::BeginPlay()
{
	Super::BeginPlay();

    //Set default character state 
    CharacterState = ECharacterState::Normal;

    OutlineCharacter();

    if(LightningTrail)
    {
        LightningTrail->Deactivate();
    }
}

void ACharacterMovement::OutlineCharacter()
{
    //Retrun if local player
    if (IsLocallyControlled())
    {
        UE_LOG(LogTemp, Warning, TEXT("lcoal player"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Outline"));
    //Get character's mesh
    if (USkeletalMeshComponent* CharacterMesh = GetMesh())
    {
        UE_LOG(LogTemp, Warning, TEXT("Got mesh"));
        //Get local player controller
        if (APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController())
        {
            UE_LOG(LogTemp, Warning, TEXT("Got local pc"));
            //Get local player state
            if (ARealmsPlayerState* LocalPlayerState = Cast<ARealmsPlayerState>(LocalPlayerController->PlayerState))
            {
                UE_LOG(LogTemp, Warning, TEXT("got local ps"));

                //Mark as an enemy if local player not in a team
                if (LocalPlayerState->GetTeam() == ETeam::NoTeam)
                {
                    UE_LOG(LogTemp, Warning, TEXT("no team"));
                    CharacterMesh->SetRenderCustomDepth(true);
                    CharacterMesh->SetCustomDepthStencilValue(2);
                }
                else
                {
                    //Get non local playuer state
                    if (ARealmsPlayerState* NonLocalPlayerState = Cast<ARealmsPlayerState>(GetPlayerState()))
                    {
                        UE_LOG(LogTemp, Warning, TEXT("got non local ps"));
                        UE_LOG(LogTemp, Warning, TEXT("Local: %d\nNonLocal: %d"), LocalPlayerState->GetTeam(), NonLocalPlayerState->GetTeam());
                        //Mark as an enemy if local player and non local player are not in the same team
                        if (LocalPlayerState->GetTeam() != NonLocalPlayerState->GetTeam())
                        {
                            UE_LOG(LogTemp, Warning, TEXT("team"));
                            CharacterMesh->SetRenderCustomDepth(true);
                            CharacterMesh->SetCustomDepthStencilValue(2);
                        }
                    }
                }
            }
        }
    }



}


void ACharacterMovement::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    //Setup UI
    Setup();

}

void ACharacterMovement::UnPossessed()
{
    //Remove UI
    Client_UnSetup_Implementation();
    Super::UnPossessed();
}


void ACharacterMovement::Setup()
{
        Client_Setup();

        CurrentHealth = MaxHealth;
        CurrentShield = MaxShield;
}

void ACharacterMovement::Client_UnSetup_Implementation()
{
    //Remove UI
    if (IsLocallyControlled())
    {
        if (PlayerHealthBar)
        {
            PlayerHealthBar->RemoveFromParent();
            PlayerHealthBar = nullptr;
        }
    }

}

void ACharacterMovement::Client_Setup_Implementation()
{
    if (AGamePlayerController* PC = GetController<AGamePlayerController>())
    {
        //Create and display health UI
        if (IsLocallyControlled() && HealthBarClass)
        {
            if ((PlayerHealthBar = CreateWidget<UHealthBar>(PC, HealthBarClass)) != nullptr) {
                PlayerHealthBar->AddToPlayerScreen();
            }

            //Set UI values For Health and Shield
            PlayerHealthBar->SetHealth(1);
            PlayerHealthBar->SetShield(1);
        }
    }
}

//Called at end of game
void ACharacterMovement::EndPlay(const EEndPlayReason::Type EndPlayReason) 
{

    Super::EndPlay(EndPlayReason);
    
    //Remove UI
    Client_UnSetup();
}


// Called every frame
void ACharacterMovement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //Increase FOV
    if (bFOVChange)
    {
        float CurrentFOV = FPSCameraComponent->FieldOfView;
        //Speed boost activated
        if (SpeedBoostMultiplier > 1)
        {
            if (CurrentFOV < 100)
            {
                FPSCameraComponent->SetFieldOfView(++CurrentFOV);
            }
            else
            {
                bFOVChange = false;
            }
        }
        //Speed boost deactivated
        else
        {
            if (CurrentFOV > 90)
            {
                FPSCameraComponent->SetFieldOfView(--CurrentFOV);
            }
            else
            {
                bFOVChange = false;
            }
        }
        
    }

}

// Called to bind functionality to input
void ACharacterMovement::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up movement bindings.
    PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterMovement::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterMovement::MoveRight);

    // Set up look bindings.
    PlayerInputComponent->BindAxis("Turn", this, &ACharacterMovement::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &ACharacterMovement::AddControllerPitchInput);

    // Set up action bindings.
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterMovement::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacterMovement::StopJump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACharacterMovement::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACharacterMovement::StopSprint);

    PlayerInputComponent->BindAction("Ability", IE_Pressed, this, &ACharacterMovement::Ability);
    PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &ACharacterMovement::Ultimate);

}

//Move character foward and backward
void ACharacterMovement::MoveForward(float Value)
{
    // Find out which way is forward and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

//Move character right and left
void ACharacterMovement::MoveRight(float Value)
{
    // Find out which way is right and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetUnitAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

//Start character jump
void ACharacterMovement::StartJump()
{
    bPressedJump = true;
}

//Stop character jump
void ACharacterMovement::StopJump()
{
    bPressedJump = false;
}

//Change speed to sprinting speed
void ACharacterMovement::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
    Server_StartSprint();
}

//Change speed to walking speed
void ACharacterMovement::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
    Server_StopSprint();
}

void ACharacterMovement::Server_StartSprint_Implementation()
{
    GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
}

void ACharacterMovement::Server_StopSprint_Implementation()
{
    GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
}

void ACharacterMovement::CharacterTakeDamage(float value)
{
    //Damage Sheild First
    //if (HasAuthority())
    {
        DamageShield(value);
    }
}

void ACharacterMovement::DamageShield(float Value)
{
    float Difference = (CurrentShield - Value);
    
    //Sheild is still left
    if (Difference > 0)
    { 
        CurrentShield = Difference;
    }
    else
    {
        //Damage health with remaining damage
        CurrentShield = 0;
        if (Difference < 0)
        {
            DamageHealth((Difference * -1));
        }
    }
    Client_SetShield();
}

void ACharacterMovement::DamageHealth(float Value)
{
    float Difference = (CurrentHealth - Value);
    //Health iis still left
    if (Difference > 0)
    {
        CurrentHealth = Difference;
    }
    else
    {
        //Character dies
        CurrentHealth = 0;
        if (Difference < 0)
        {
            Die();
        }
    }
    Client_SetHealth();
}

void ACharacterMovement::GainHealth(float Value)
{
    float Addition = (CurrentHealth + Value);
    //Maximum health is reached
    if (Addition >= MaxHealth)
    {
        CurrentHealth = MaxHealth;
    }
    //Maximum Health is not reached
    else
    {
        CurrentHealth = Addition;
    }
    Client_SetHealth();
}

void ACharacterMovement::GainShield(float value)
{
    float Addition = (CurrentShield + value);
    if (Addition >= MaxShield)
    {
        CurrentShield = MaxShield;
    }
    else
    {
        CurrentShield = Addition;
    }
    Client_SetShield();
}
//

void ACharacterMovement::MultiplyDamage(float Value)
{
    if (HasAuthority())
    {
        DamageBoostMultiplier *= Value;

        if (!IsLocallyControlled())
        {
            //Client
            Client_SetDamage(Value);
        }
        GetWorldTimerManager().SetTimer(TimerHandle, this, &ACharacterMovement::ResetDamage, 1.0f, false, 3.0f);
    }
}

void ACharacterMovement::ResetDamage()
{
    if (HasAuthority())
    {
        //Server
        DamageBoostMultiplier = 1;
        if (!IsLocallyControlled())
        {
            //Client
            Client_ResetDamage();

        }
    }
}

void ACharacterMovement::Client_SetDamage_Implementation(float PlayerDamageMultiplier = 1)
{
    if (IsLocallyControlled() && PlayerHealthBar)
    {
        DamageBoostMultiplier = PlayerDamageMultiplier;

        PlayerHealthBar->ShowStrength();
    }
}

void ACharacterMovement::Client_ResetDamage_Implementation()
{
    if (IsLocallyControlled() && PlayerHealthBar)
    {
        GetCharacterMovement()->MaxWalkSpeed /= DamageBoostMultiplier;

        PlayerHealthBar->HideStrength();
    }
}


void ACharacterMovement::MultiplySpeed(float Value)
{
    if (HasAuthority())
    {
        
        //Clear current speed multiplier and multiply with new multiplyer
        GetCharacterMovement()->MaxWalkSpeed /= SpeedBoostMultiplier;
        SpeedBoostMultiplier = Value;
        GetCharacterMovement()->MaxWalkSpeed *= SpeedBoostMultiplier;


        if (LightningTrail)
        {
            LightningTrail->Activate();
        }
        bLightningEffectActive = true;

        Client_SetSpeed(Value);

        GetWorldTimerManager().SetTimer(TimerHandle, this, &ACharacterMovement::ResetSpeed, 1.0f, false, 5.0f);
    }    
}

void ACharacterMovement::ResetSpeed()
{
    if (HasAuthority())
    {
        //Clear speed boost
        GetCharacterMovement()->MaxWalkSpeed /= SpeedBoostMultiplier;
        SpeedBoostMultiplier = 1;

        if (LightningTrail)
        {
            LightningTrail->Deactivate();
        }
        bLightningEffectActive = false;
        Client_ResetSpeed();
    }
}

void ACharacterMovement::Client_SetSpeed_Implementation(float PlayerSpeedMultiplier = 1)
{
    if (IsLocallyControlled() && PlayerHealthBar)
    {
       //GetCharacterMovement()->MaxWalkSpeed /= SpeedBoostMultiplier;
        SpeedBoostMultiplier = PlayerSpeedMultiplier;
        GetCharacterMovement()->MaxWalkSpeed *= SpeedBoostMultiplier;
        bFOVChange = true;

        PlayerHealthBar->ShowSpeed();
    }
}

void ACharacterMovement::Client_ResetSpeed_Implementation()
{
    if (IsLocallyControlled() && PlayerHealthBar)
    {
        SpeedBoostMultiplier = 1;
        GetCharacterMovement()->MaxWalkSpeed /= SpeedBoostMultiplier;
        bFOVChange = true;

        PlayerHealthBar->HideSpeed();
    }
}

void ACharacterMovement::OnRep_LightningEffectActive()
{
    if (bLightningEffectActive)
    {
        // Activate lightning effect
        if (LightningTrail)
        {
            LightningTrail->Activate();
        }
    }
    else
    {
        // Deactivate lightning effect
        if (LightningTrail)
        {
            LightningTrail->Deactivate();
        }
    }
}

void ACharacterMovement::Client_SetHealth_Implementation()
{
    if (IsLocallyControlled() && PlayerHealthBar)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Health Set: %f"), currentHealth);
        PlayerHealthBar->SetHealth((CurrentHealth > 0) ? CurrentHealth / MaxHealth : 0);
    }
}
void ACharacterMovement::Client_SetShield_Implementation()
{
    if (IsLocallyControlled() && PlayerHealthBar)
    {
       // UE_LOG(LogTemp, Warning, TEXT("Shield Set: %f"), currentShield);
        PlayerHealthBar->SetShield((CurrentShield > 0) ? CurrentShield / MaxShield : 0);
    }
}


void ACharacterMovement::MenuMode()
{
    if (AGamePlayerController* PC = GetController<AGamePlayerController>())
    {
        //Set input mode to game and ui and set cursor to visible 
        FInputModeGameAndUI GameAndUI;
        PC->SetInputMode(GameAndUI);
        PC->SetShowMouseCursor(true);
    }
}

void ACharacterMovement::PlayMode()
{
    if(AGamePlayerController* PC = GetController<AGamePlayerController>())
    {
        //Set input mode to game only and set cursor to invisible 
        FInputModeGameOnly GameOnly;
        PC->SetInputMode(GameOnly);
        PC->SetShowMouseCursor(false);
    }

}

//Character set chracter's state
void ACharacterMovement::SetCharacterState(ECharacterState State)
{
    UE_LOG(LogTemp, Warning, TEXT("Setting Character State"));
    CharacterState = State;
    OnRep_HandleCharacterState();
}

//Get chracter's state
ECharacterState ACharacterMovement::GetCharacterState()
{
    return CharacterState;
}

//Handle change in character's state
void ACharacterMovement::OnRep_HandleCharacterState()
{
    if (CharacterState == ECharacterState::Dead)
    {
        DeadState();
    }
}

//Handle character death
void ACharacterMovement::Die()
{

    if (HasAuthority())
    {
        SetCharacterState(ECharacterState::Dead);
        //Record death
        if (ARealmsPlayerState* State = this->GetPlayerState<ARealmsPlayerState>())
        {
            State->AddDeath();
        }
        //Kill player
        if (AGamePlayerController* PC = GetController<AGamePlayerController>())
        {
            PC->Die();
        }
    }
}

//
void ACharacterMovement::DeadState()
{
    //Simulate physics
    this->GetMesh()->SetSimulatePhysics(true);
    this->GetMesh()->SetCollisionProfileName(TEXT("PhysicsActor"));
    
    //Display mesh to player
    this->GetMesh()->SetOwnerNoSee(false);

    //Destroy character in 5 seconds
    if (HasAuthority())
    {
        this->SetLifeSpan(5);
    }
}

void ACharacterMovement::Ability()
{

}

void ACharacterMovement::Ultimate()
{

}