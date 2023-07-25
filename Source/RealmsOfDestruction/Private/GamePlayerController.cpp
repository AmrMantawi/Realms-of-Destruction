// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectionMenu.h"
#include "CharacterMovement.h"
#include "RealmsGameMode.h"
#include "RealmsGameState.h"
#include "CharacterSelectionMenu.h"
#include "PlayesrDisplay.h"
#include "SettingsMenu.h"
#include "PauseMenu.h"
#include "PlayesrDisplay.h"
#include "Net/UnrealNetwork.h"

AGamePlayerController::AGamePlayerController()
{
	bReplicates = true;
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Player", IE_Pressed, this, &AGamePlayerController::TogglePlayerDisplay);
	InputComponent->BindAction("Pause Game", IE_Pressed, this, &AGamePlayerController::TogglePauseMenu);
}

void AGamePlayerController::BeginPlay() {
	Super::BeginPlay();

	//Setup UI
	Client_Setup();
	Server_GetPlayerDisplayClass();
}

void AGamePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Client_UnSetup();
}

void AGamePlayerController::Client_Setup_Implementation()
{
	//Create character selection UI
	if (CharacterSelectionClass)
	{
		PlayerCharacterSelection = CreateWidget<UCharacterSelectionMenu>(this, CharacterSelectionClass);
	}

	//Setup settings menu
	if (SettingsMenuClass)
	{
		PlayerSettingsMenu = CreateWidget<USettingsMenu>(this, SettingsMenuClass);
	}

	//Setup pause menu
	if (PauseMenuClass)
	{
		PlayerPauseMenu = CreateWidget<UPauseMenu>(this, PauseMenuClass);
		//Setup pause menu button functions
		if (PlayerPauseMenu != nullptr)
		{
			PlayerPauseMenu->Resume->OnClicked.AddDynamic(this, &AGamePlayerController::TogglePauseMenu);
			PlayerPauseMenu->Settings->OnClicked.AddDynamic(this, &AGamePlayerController::ToggleSettingsMenu);
			PlayerPauseMenu->ChangeCharacter->OnClicked.AddDynamic(this, &AGamePlayerController::ToggleCharacterSelectionMenu);
		}
	}
}

void AGamePlayerController::Client_UnSetup_Implementation()
{
	if (PlayerPauseMenu)
	{
		PlayerPauseMenu->RemoveFromParent();
		PlayerPauseMenu = nullptr;
	}
	if (PlayerSettingsMenu)
	{
		PlayerSettingsMenu->RemoveFromParent();
		PlayerSettingsMenu = nullptr;
	}
	if (PlayerCharacterSelection)
	{
		PlayerCharacterSelection->RemoveFromParent();
		PlayerCharacterSelection = nullptr;
	}
	if(PlayersDisplay)
	{
		PlayersDisplay->RemoveFromParent();
		PlayersDisplay = nullptr;
	}
}

void AGamePlayerController::Client_CharacterSelection_Implementation()
{
	Server_CharacterSelection();

	//Hide pause menu
	if (PlayerPauseMenu)
	{
		PlayerPauseMenu->RemoveFromParent();
		bPauseMenuDisplayed = false;
	}
	//Hide settings menu
	if (PlayerSettingsMenu)
	{
		PlayerSettingsMenu->RemoveFromParent();
		bSettingsDisplayed = false;
	}
	//Hide player list menu
	if (PlayersDisplay)
	{
		PlayersDisplay->RemoveFromParent();
		bPlayersMenuDisplayed = false;
	}
	//Display character selection menu if not already displayed
	if (!bCharacterSelectionMenuDisplayed && PlayerCharacterSelection)
	{
		PlayerCharacterSelection->AddToPlayerScreen();
		bCharacterSelectionMenuDisplayed = true;
	}
}

void AGamePlayerController::Server_CharacterSelection_Implementation()
{
	//Destroy possessed pawn if present
	if (CurrentCharacter || this->GetPawn())
	{
		this->GetPawn()->Destroy();
		this->UnPossess();
	}

	//Set spawn parameters
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	APawn* CharacterSelectPawn = GetWorld()->SpawnActor<APawn>(APawn::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
	this->Possess(CharacterSelectPawn);
}

void AGamePlayerController::ToggleCharacterSelectionMenu()
{
	//Toggle character selection menu
	if (PlayerCharacterSelection)
	{
		//Diplay UI
		if (!bCharacterSelectionMenuDisplayed)
		{
			PlayerCharacterSelection->AddToPlayerScreen();
			bCharacterSelectionMenuDisplayed = true;
		}
		//Hide UI
		else
		{
			PlayerCharacterSelection->RemoveFromParent();
			bCharacterSelectionMenuDisplayed = false;
		}
	}
}

//Toggle pause menu
void AGamePlayerController::TogglePauseMenu()
{
	//Toggle pause menu
	if (PlayerPauseMenu)
	{
		if (!bPauseMenuDisplayed)
		{
			//Hide character selection menu if displayed
			if (bCharacterSelectionMenuDisplayed)
			{
				PlayerPauseMenu->ChangeCharacter->SetVisibility(ESlateVisibility::Hidden);
			}
			//Display pause if character selection menu not displyed
			else
			{
				PlayerPauseMenu->ChangeCharacter->SetVisibility(ESlateVisibility::Visible);
				PlayerPauseMenu->AddToPlayerScreen();
				bPauseMenuDisplayed = true;

				FInputModeGameAndUI GameAndUI;
				this->SetInputMode(GameAndUI);
				this->SetShowMouseCursor(true);
			}
		}
		else
		{
			//Hide settings menu
			if (bSettingsDisplayed)
			{
				PlayerSettingsMenu->RemoveFromParent();
				bSettingsDisplayed = false;
			}
			//Hide character selection menu
			else if (bCharacterSelectionMenuDisplayed)
			{
				PlayerCharacterSelection->RemoveFromParent();
				bCharacterSelectionMenuDisplayed = false;
			}
			//Hide pause menu
			else {
				PlayerPauseMenu->RemoveFromParent();
				bPauseMenuDisplayed = false;

				FInputModeGameOnly GameOnly;
				this->SetInputMode(GameOnly);
				this->SetShowMouseCursor(false);
			}
		}
	}
}


void AGamePlayerController::ToggleSettingsMenu()
{
	//Toggle settings menu
	if (PlayerSettingsMenu)
	{
		//Diplay UI
		if (!bSettingsDisplayed)
		{
			PlayerSettingsMenu->AddToPlayerScreen();
			bSettingsDisplayed = true;
		}
		//Hide UI
		else
		{
			PlayerSettingsMenu->RemoveFromParent();
			bSettingsDisplayed = false;
		}
	}
}

void AGamePlayerController::Server_GetPlayerDisplayClass_Implementation()
{
	//Get player display class from game mode
	if (ARealmsGameMode* GameMode = Cast<ARealmsGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (TSubclassOf<class UPlayesrDisplay> PlayersDisplayClass = GameMode->GetPlayerDisplayClass())
		{
			Client_GetPlayerDisplayClass(PlayersDisplayClass);
		}
	}
}

void AGamePlayerController::Client_GetPlayerDisplayClass_Implementation(TSubclassOf<class UPlayesrDisplay> PlayersDisplayClass)
{
	//Create Player Display Widget
	PlayersDisplay = CreateWidget<UPlayesrDisplay>(this, PlayersDisplayClass);
}

void AGamePlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGamePlayerController, CurrentCharacter);
}

void AGamePlayerController::SelectCharacter(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint)
{
	//Update selected character
	SelectedCharacter = SelectedCharacterBlueprint;
}

void AGamePlayerController::SpawnCharacter()
{
	if (SelectCharacter == nullptr)
		return;

	//Spawn character if in a valid match state for character spawning
	if (ARealmsGameState* GameState = Cast<ARealmsGameState>(GetWorld()->GetGameState()))
	{
		EMatchState MatchState = GameState->GetMatchState();
		if (MatchState == EMatchState::Not_Started || MatchState == EMatchState::In_Progress)
		{
			//Hide UI
			if (bCharacterSelectionMenuDisplayed && PlayerCharacterSelection)
			{
				PlayerCharacterSelection->RemoveFromParent();
				bCharacterSelectionMenuDisplayed = false;
			}

			//Spawn chracter and set input mode to game only
			FInputModeGameOnly GameOnly;
			this->SetInputMode(GameOnly);
			this->SetShowMouseCursor(false);
			Server_SpawnCharacter(SelectedCharacter);
		}
	}
}

void AGamePlayerController::Server_SpawnCharacter_Implementation(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint)
{
	//Destroy pawn if is not a character
	if (!CurrentCharacter && this->GetPawn())
	{
		this->GetPawn()->Destroy();
	}
	this->UnPossess();

	//Set spawn parameters
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//Spawn selected chracter and possess it
	if(SelectedCharacterBlueprint)
	{
		CurrentCharacter = GetWorld()->SpawnActor<ACharacterMovement>(SelectedCharacterBlueprint, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
		this->Possess(CurrentCharacter);
	}
	
}

void AGamePlayerController::TogglePlayerDisplay()
{
	//Toggle player list display
	if (PlayersDisplay)
	{
		//Diplay UI
		if (!bPlayersMenuDisplayed)
		{
			PlayersDisplay->AddToPlayerScreen();
		}
		//Hide UI
		else
		{
			PlayersDisplay->RemoveFromParent();
		}

		bPlayersMenuDisplayed = !bPlayersMenuDisplayed;
	}
}

void AGamePlayerController::Die()
{
	//Respawn character
	Server_SpawnCharacter(SelectedCharacter);
}

void AGamePlayerController::DisplayEndGameScreen(bool bIsWinner)
{
	//Display winning/losing UI
	Server_DisplayEndGameScreen(bIsWinner);
}

void AGamePlayerController::Server_DisplayEndGameScreen_Implementation(bool bIsWinner)
{
	//Get end game UI class
	if (ARealmsGameMode* GameMode = Cast<ARealmsGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (TSubclassOf<class UUserWidget> EndGameScreenClass = GameMode->GetEndGameUIClass(bIsWinner))
		{
			//Display end game UI to owning player
			Client_DisplayEndGameScreen(EndGameScreenClass);
		}
	}
}

void AGamePlayerController::Client_DisplayEndGameScreen_Implementation(TSubclassOf<class UUserWidget> EndGameScreenClass)
{
	//Remove existing end game screen if present
	if (EndGameScreen)
		EndGameScreen->RemoveFromParent();

	//Create and display end game ui widget
	EndGameScreen = CreateWidget<UUserWidget>(this, EndGameScreenClass);
	if (EndGameScreen)
	{
		EndGameScreen->AddToPlayerScreen();
	}
}
