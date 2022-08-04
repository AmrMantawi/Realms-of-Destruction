// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerController.h"

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
	this->SetInputMode(GameOnly);
	this->SetShowMouseCursor(false);

	if (HasAuthority())
	{
		this->UnPossess();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		currentCharacter = GetWorld()->SpawnActor<ACharacterMovement>(SelectedCharacter, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
		this->Possess(currentCharacter);
		currentCharacter->Setup();
	}
	else
	{
		Server_SpawnCharacter(FVector::ZeroVector, FRotator::ZeroRotator);
	}
}

void AGamePlayerController::Server_SpawnCharacter_Implementation(FVector Location, FRotator Rotation)
{
	this->UnPossess();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APawn* character = GetWorld()->SpawnActor<APawn>(SelectedCharacter, Location, Rotation, SpawnParameters);
	this->Possess(character);
	//character->Setup();

}


/*
void AGamePlayerController::ToggleCharacterSelectionMenu(bool toggle)
{

	//Toggle On
	if (true)
	{

	}
	//Toggle Off
	else
	{

	}

	ToggleUIMode(toggle);
}

void AGamePlayerController::TogglePauseMenu(bool toggle)
{
	//Toggle On
	if (true)
	{

	}
	//Toggle Off
	else
	{

	}
	ToggleUIMode(toggle);
}
*/

void AGamePlayerController::ToggleUIMode(bool toggle)
{
	//Toggle On
	if (true)
	{
		this->SetInputMode(UIOnly);
		this->SetShowMouseCursor(true);
	}
	//Toggle Off
	else
	{
		this->SetInputMode(GameOnly);
		this->SetShowMouseCursor(false);
	}
}

void AGamePlayerController::Die()
{
	//Make Character Despawn After 5 seconds
	currentCharacter->SetLifeSpan(5);
}