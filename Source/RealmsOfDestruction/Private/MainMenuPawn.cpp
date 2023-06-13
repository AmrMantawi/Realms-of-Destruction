// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPawn.h"

// Sets default values
AMainMenuPawn::AMainMenuPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

void AMainMenuPawn::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    PC = GetController<APlayerController>();
    check(PC);

    if (PC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Menu Mode"));

        FInputModeGameAndUI GameAndUIMode;

        //Create UI when pawn is possessed
        PC->SetInputMode(GameAndUIMode);
        PC->SetShowMouseCursor(true);
        if (MainMenuClass)
        {
            //Create Main Menu

            playerMainMenu = CreateWidget<UMainMenu>(PC, MainMenuClass);
            playerMainMenu->AddToPlayerScreen();

            if (SettingsMenuClass)
            {
                //Create Pause Menu
                playerSettingsMenu = CreateWidget<USettingsMenu>(PC, SettingsMenuClass);
                playerMainMenu->SettingsButton->OnClicked.AddDynamic(this, &AMainMenuPawn::ToggleSettingsMenu);
            }
        }
    }
}

// Called when the game starts or when spawned
void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Pause Game", IE_Pressed, this, &AMainMenuPawn::ToggleSettingsMenu);
}

// Toggle Settings Menu On and OFF
void AMainMenuPawn::ToggleSettingsMenu()
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
