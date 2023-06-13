// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectionMenu.h"
#include "PauseMenu.h"
#include "CharacterMovement.h"
#include "RealmsGameMode.h"
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
}

void AGamePlayerController::BeginPlay() {
	Super::BeginPlay();

	//Setup UI
	Server_GetPlayerDisplayClass();
}

void AGamePlayerController::Server_GetPlayerDisplayClass_Implementation()
{
	if (ARealmsGameMode* GameMode = Cast<ARealmsGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (TSubclassOf<class UPlayesrDisplay> PlayersDisplayClass = GameMode->GetPlayerDisplayClass())
		{
			Client_Setup(PlayersDisplayClass);
		}
	}
}

void AGamePlayerController::Client_Setup_Implementation(TSubclassOf<class UPlayesrDisplay> PlayersDisplayClass)
{
	//Create Player Display Widget
	PlayersDisplay = CreateWidget<UPlayesrDisplay>(this, PlayersDisplayClass);
}

void AGamePlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGamePlayerController, currentCharacter);
}

void AGamePlayerController::SelectCharacter(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint)
{
	SelectedCharacter = SelectedCharacterBlueprint;

	if (!HasAuthority())
	{
		Server_SelectCharacter(SelectedCharacterBlueprint);
	}
}

void AGamePlayerController::Server_SelectCharacter_Implementation(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint)
{
	SelectedCharacter = SelectedCharacterBlueprint;
}

void AGamePlayerController::SpawnCharacter()
{
	FInputModeGameOnly GameOnly;
	this->SetInputMode(GameOnly);
	this->SetShowMouseCursor(false);
	Server_SpawnCharacter(FVector::ZeroVector, FRotator::ZeroRotator);

	//if (HasAuthority())
	//{
	//	if (this->GetPawn())
	//	{
	//		this->GetPawn()->Destroy();
	//	}
	//	this->UnPossess();
	//	FActorSpawnParameters SpawnParameters;
	//	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//	currentCharacter = GetWorld()->SpawnActor<ACharacterMovement>(SelectedCharacter, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
	//	this->Possess(currentCharacter);

	//}
	//else
	//{

	//}
}

void AGamePlayerController::Server_SpawnCharacter_Implementation(FVector Location, FRotator Rotation)
{
	if (!currentCharacter && this->GetPawn())
	{
		this->GetPawn()->Destroy();
	}
	this->UnPossess();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (currentCharacter)
	{
		ACharacterMovement* newCharacter = GetWorld()->SpawnActor<ACharacterMovement>(SelectedCharacter, Location, Rotation, SpawnParameters);
		this->Possess(newCharacter);
		currentCharacter->setCharacterState(ECharacterState::Dead);
		currentCharacter = newCharacter;

	}
	else {
		currentCharacter = GetWorld()->SpawnActor<ACharacterMovement>(SelectedCharacter, Location, Rotation, SpawnParameters);
		this->Possess(currentCharacter);
	}

}

void AGamePlayerController::TogglePlayerDisplay()
{
	if (PlayersDisplay)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Display Set"));

		if (!bPlayersMenuDisplayed)
		{
			PlayersDisplay->AddToPlayerScreen();
		}
		else
		{
			PlayersDisplay->RemoveFromParent();
		}

		bPlayersMenuDisplayed = !bPlayersMenuDisplayed;
	}
}

void AGamePlayerController::Die()
{
	Server_SpawnCharacter(FVector::ZeroVector, FRotator::ZeroRotator);
}

