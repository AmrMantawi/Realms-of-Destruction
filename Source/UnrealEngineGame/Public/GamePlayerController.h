// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterMovement.h"
//#include "CharacterSelectionMenu.h"
#include "GamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALENGINEGAME_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
		 
public:


	//Select Character
	void SelectCharacter(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint);

	UFUNCTION(Reliable, Server)
	void Server_SelectCharacter(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint);
	void Server_SelectCharacter_Implementation(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint);

	//Spawn Character and Possess
	void SpawnCharacter();

	UFUNCTION(Reliable, Server)
	void Server_SpawnCharacter(FVector Location, FRotator Rotation);
	void Server_SpawnCharacter_Implementation(FVector Location, FRotator Rotation);

	//Toggle Character Selection Menu
	//void ToggleCharacterSelectionMenu(bool toggle);

	//Toggle Pause Menu
	//void TogglePauseMenu(bool toggle);

	//Toggle Mouse Visibility and Input Mode
	void ToggleUIMode(bool toggle);

private:

	UPROPERTY()
	TSubclassOf<class ACharacterMovement> SelectedCharacter;

	//UPROPERTY()
	//TSubclassOf<class UCharacterSelectionMenu> SelectedCharacter;



	FInputModeUIOnly UIOnly;
	FInputModeGameOnly GameOnly;
};
