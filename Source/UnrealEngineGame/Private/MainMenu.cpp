// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "EOSGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"


void UMainMenu::MenuSetup()
{
	EOSInstance = GetGameInstance<UEOSGameInstance>();
}


void UMainMenu::DisplaySessions(UPanelWidget* SessionEntryPanel)
{
	if(EOSInstance)
	{

		EOSInstance->FindSessions(SessionEntryPanel, EntryClass);
		/*
		SessionIndex = 0;
		UE_LOG(LogTemp, Warning, TEXT("Success: %d Sessions"), AvailableSessions.Num());
		for (FOnlineSessionSearchResult UserSession : AvailableSessions)
		{
			SessionEntry = CreateWidget<UEntry>(this, EOSInstance->FindSessions());
			SessionEntryPanel->AddChild(SessionEntry);
			//SessionEntry->Name->Text = FText::FromString(UserSession.Session.OwningUserName);
			//SessionEntry->Capacity->Text = FText::FromString(FString::FromInt(UserSession.Session.NumOpenPublicConnections));
			//SessionEntry->JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinUserSession);
			UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), SessionIndex);
			SessionIndex++;
		}
		*/
	}
}

void UMainMenu::JoinUserSession()
{
	//EOSInstance->JoinUserSession(SessionIndex);
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	MenuTearDown();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMainMenu::MenuTearDown()
{
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
