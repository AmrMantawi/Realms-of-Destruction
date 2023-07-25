// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterMovement.h"
#include "GamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
		 
public:

	AGamePlayerController();

	//Get Player Display Class
	UFUNCTION(Client, Reliable)
	void Client_GetPlayerDisplayClass(TSubclassOf<class UPlayesrDisplay> PlayersDisplayClass);
	void Client_GetPlayerDisplayClass_Implementation(TSubclassOf<class UPlayesrDisplay> PlayersDisplayClass);
	UFUNCTION(Server, Reliable)
	void Server_GetPlayerDisplayClass();
	void Server_GetPlayerDisplayClass_Implementation();

	//Select Character
	UFUNCTION()
	void SelectCharacter(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint);

	//Spawn pawn for character selection
	UFUNCTION(Client, Reliable)
	void Client_CharacterSelection();
	void Client_CharacterSelection_Implementation();


	//Spawn Character and Possess it
	UFUNCTION()
	void SpawnCharacter();
	UFUNCTION(Reliable, Server)
	void Server_SpawnCharacter(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint);
	void Server_SpawnCharacter_Implementation(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint);

	//Handle Player Death
	UFUNCTION()
	void Die();

	//Toggle player display UI
	UFUNCTION()
	void TogglePlayerDisplay();

	//Display winning/ losing screen
	UFUNCTION()
	void DisplayEndGameScreen(bool bIsWinner);

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Setup Player Controller 
	UFUNCTION(Client, Reliable)
	void Client_Setup();
	void Client_Setup_Implementation();


	UFUNCTION(Client, Reliable)
	void Client_UnSetup();
	void Client_UnSetup_Implementation();

	//Display End Game UI
	UFUNCTION(Client, Reliable)
	void Client_DisplayEndGameScreen(TSubclassOf<class UUserWidget> EndGameScreenClass);
	void Client_DisplayEndGameScreen_Implementation(TSubclassOf<class UUserWidget> EndGameScreenClass);

	//Get End Game UI
	UFUNCTION(Server, Reliable)
	void Server_DisplayEndGameScreen(bool bIsWinner);
	void Server_DisplayEndGameScreen_Implementation(bool bIsWinner);

	UFUNCTION()
	void ToggleCharacterSelectionMenu();

	UFUNCTION()
	void TogglePauseMenu();

	UFUNCTION()
	void ToggleSettingsMenu();

	UFUNCTION(Server, Reliable)
	void Server_CharacterSelection();
	void Server_CharacterSelection_Implementation();

private:
	UPROPERTY()
	TSubclassOf<class ACharacterMovement> SelectedCharacter;

	UPROPERTY()
	class UPauseMenu* PlayerPauseMenu;

	UPROPERTY()
	class UCharacterSelectionMenu* PlayerCharacterSelection;

	UPROPERTY()
	class USettingsMenu* PlayerSettingsMenu;

	UPROPERTY()
	class UUserWidget* EndGameScreen;

	//Player Display UI
	UPROPERTY()
	class UPlayesrDisplay* PlayersDisplay;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCharacterSelectionMenu> CharacterSelectionClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UPauseMenu> PauseMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USettingsMenu> SettingsMenuClass;

	UPROPERTY()
	bool bPauseMenuDisplayed = false;

	UPROPERTY()
	bool bSettingsDisplayed = false;

	UPROPERTY()
	bool bCharacterSelectionMenuDisplayed = false;

	UPROPERTY()
	bool bPlayersMenuDisplayed = false;

	//Current selected character
	UPROPERTY(Replicated)
	ACharacterMovement* CurrentCharacter;
};
