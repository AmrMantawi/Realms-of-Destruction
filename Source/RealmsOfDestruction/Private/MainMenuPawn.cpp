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

    if (APlayerController* PC = GetController<APlayerController>())
    {
        UE_LOG(LogTemp, Warning, TEXT("Menu Mode"));

        FInputModeGameAndUI GameAndUIMode;

        //Create UI when pawn is possessed
        PC->SetInputMode(GameAndUIMode);
        PC->SetShowMouseCursor(true);
        if (MainMenuClass)
        {
            //Create Main Menu

            PlayerMainMenu = CreateWidget<UMainMenu>(PC, MainMenuClass);
            PlayerMainMenu->AddToPlayerScreen();
        }
    }
}

// Called when the game starts or when spawned
void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	
    if (IsLocallyControlled())
    {
        if (APlayerController* PC = GetController<APlayerController>())
        {
            PC->SetInputMode(FInputModeGameAndUI());
        }
    }
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
	PlayerInputComponent->BindAction("Pause Game", IE_Pressed, this, &AMainMenuPawn::TogglePauseMenu);
}

// Toggle Pause Menu On and OFF
void AMainMenuPawn::TogglePauseMenu()
{
    if (PlayerMainMenu)
    {
        PlayerMainMenu->TogglePauseMenu();
    }
}