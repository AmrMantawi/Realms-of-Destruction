// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "EOSGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"


void UMainMenu::MenuSetup()
{
	//Set player user name
	EOSInstance = GetGameInstance<UEOSGameInstance>();
	if (EOSInstance)
	{
		UserNameText->SetText(FText::FromString(EOSInstance->PlayerUserName));
	}
}


void UMainMenu::DisplaySessions(UPanelWidget* SessionEntryPanel)
{
	//Fill in contents of session entry panel with available entries
	if(EOSInstance)
	{
		EOSInstance->FindCustomGameSessions(SessionEntryPanel, EntryClass);
	}
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	//Clear UI
	MenuTearDown();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMainMenu::MenuTearDown()
{
	//Clear UI
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void UMainMenu::PracticeGame() 
{
	if (UWorld* World = GetWorld())
	{
		World->ServerTravel("/Game/Maps/PracticeMap");
	}
}