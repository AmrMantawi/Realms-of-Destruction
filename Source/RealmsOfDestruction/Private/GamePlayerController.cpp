// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectionMenu.h"
#include "PauseMenu.h"

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

	if (HasAuthority())
	{
		if (this->GetPawn())
		{
			this->GetPawn()->Destroy();
		}
		this->UnPossess();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		currentCharacter = GetWorld()->SpawnActor<ACharacterMovement>(SelectedCharacter, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);
		this->Possess(currentCharacter);

	}
	else
	{
		Server_SpawnCharacter(FVector::ZeroVector, FRotator::ZeroRotator);
	}
}

void AGamePlayerController::Server_SpawnCharacter_Implementation(FVector Location, FRotator Rotation)
{
	if (this->GetPawn())
	{
		this->GetPawn()->Destroy();
	}
	this->UnPossess();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	currentCharacter = GetWorld()->SpawnActor<ACharacterMovement>(SelectedCharacter, Location, Rotation, SpawnParameters);
	this->Possess(currentCharacter);
}

void AGamePlayerController::Die()
{
	//Make Character Despawn After 5 seconds
	//currentCharacter->SetLifeSpan(5);
	UE_LOG(LogTemp, Warning, TEXT("PC Dead"));

}