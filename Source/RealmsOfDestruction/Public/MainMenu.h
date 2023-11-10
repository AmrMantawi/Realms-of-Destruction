// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "SettingsMenu.h"
#include "EOSGameInstance.h"
#include "Entry.h"
#include "MainMenu.generated.h"

/**
 * 
 */

UCLASS()
class REALMSOFDESTRUCTION_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	//Set default values of menu
	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void MenuSetup();

	//Display available sessions
	UFUNCTION(BlueprintCallable)
	void DisplaySessions(UPanelWidget* SessionEntryPanel);

	class UEOSGameInstance* EOSInstance;

	//Class of entry for server list
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UEntry> EntryClass;
	UPROPERTY()
	class UEntry* SessionEntry;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* UserNameText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget), BlueprintReadWrite)
	UBorder* PauseMenu;

	//Trasport player to practice level
	UFUNCTION(BlueprintCallable, Category = "Game Manager")
	void PracticeGame();

	UFUNCTION()
	void TogglePauseMenu();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USettingsMenu* W_Settings;

	UFUNCTION()
	void ToggleSettingsMenu();

private:
	//Handle Menu teardown
	void MenuTearDown();

protected:
	class IOnlineSubsystem* OnlineSubsystem;

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

};
