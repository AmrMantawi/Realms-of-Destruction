// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

const FName TestSessionName = FName("RealmsOfDestructionLobby");

UEOSGameInstance::UEOSGameInstance()
{
	bIsLoggedIn = false;
	//PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
}

void UEOSGameInstance::Init()
{
	Super::Init();

	OnlineSubsystem = IOnlineSubsystem::Get();
	Login();
}

void UEOSGameInstance::Login()
{
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			FOnlineAccountCredentials Credentials;
			Credentials.Id = FString("");
			Credentials.Token = FString("");
			Credentials.Type = FString("accountportal");

			Identity->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::OnLoginComplete);
			Identity->Login(0, Credentials);
		}
	}
}

void UEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	UE_LOG(LogTemp, Warning, TEXT("LoggedIn: %d"), bWasSuccessful);
	bIsLoggedIn = bWasSuccessful;

	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			Identity->ClearOnLoginCompleteDelegates(0, this);

			PlayerUserName = Identity->GetPlayerNickname(UserId);

			UE_LOG(LogTemp, Warning, TEXT("User Name: %s"), *PlayerUserName);

			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				PC->ClientTravel("/Game/Maps/TestMenu", TRAVEL_Absolute);
			}
		}
	}
}

void UEOSGameInstance::CreateSession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				FOnlineSessionSettings SessionSettings;
				SessionSettings.bIsDedicated = false;
				SessionSettings.bShouldAdvertise = true;
				SessionSettings.bIsLANMatch = false;
				SessionSettings.NumPublicConnections = 5;
				SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bUsesPresence = true;
				SessionSettings.Set(SEARCH_KEYWORDS, FString("RealmsOfDestructionLobby"), EOnlineDataAdvertisementType::ViaOnlineService);
				SessionSettings.bUseLobbiesIfAvailable = true;
				SessionSettings.BuildUniqueId = 1;

				const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
				SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnCreateSessionComplete);
				SessionPtr->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionSettings);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Create Session : Not Logged In"));
	}
}

void UEOSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Success: %d"), bWasSuccessful);

	if (bWasSuccessful)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			if (APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				UWorld* World = GetWorld();
				if (!ensure(World != nullptr)) return;
				World->ServerTravel("/Game/Maps/FreeForAllLevel?listen");
			}
			SessionPtr->StartSession(SessionName);
			SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
		}
	}
}

void UEOSGameInstance::DestroySession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnDestroySessionComplete);
				SessionPtr->DestroySession(NAME_GameSession);
				if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
				{
					PC->ClientTravel("/Game/Maps/TestMenu", TRAVEL_Absolute);
				}
			}
		}
	}
}

void UEOSGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnDestroySessionCompleteDelegates(this);
		}
	}
}

void UEOSGameInstance::FindSessions(UPanelWidget* SessionPanel, TSubclassOf<class UEntry> EntryClassSet)
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SessionPanel->ClearChildren();

				SearchSettings = MakeShareable(new FOnlineSessionSearch());
				SearchSettings->MaxSearchResults = 5000;
				SearchSettings->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

				const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
				SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnFindSessionsComplete);
				SessionPtr->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SearchSettings.ToSharedRef());

				SessionEntryPanel = SessionPanel;
				EntryClass = EntryClassSet;
			}
		}
	}
}

void UEOSGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success: %d Lobbies"), SearchSettings->SearchResults.Num());

		TArray<class FOnlineSessionSearchResult> AvailableSessions = SearchSettings->SearchResults;

		FOnlineSessionSearchResult UserSession;
		for (int i = 0; i < AvailableSessions.Num(); i++)
		{

			UserSession = AvailableSessions[i];
			SessionEntry = CreateWidget<UEntry>(SessionEntryPanel, EntryClass);
			SessionEntryPanel->AddChild(SessionEntry);
			SessionEntry->Name->SetText(FText::FromString(UserSession.Session.OwningUserName));
			SessionEntry->Capacity->SetText(FText::FromString(FString::FromInt(UserSession.Session.NumOpenPublicConnections)));
			SessionIndex = i;
			SessionEntry->JoinButton->OnClicked.AddDynamic(this, &UEOSGameInstance::JoinSessions);
		}
	}

	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnFindSessionsCompleteDelegates(this);
		}
	}
}

void UEOSGameInstance::JoinSessions()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
				SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnJoinSessionsComplete);
				SessionPtr->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SearchSettings->SearchResults[SessionIndex]);
			}
		}
	}
}

void UEOSGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			FString ConnectionInfo = FString();
			SessionPtr->GetResolvedConnectString(NAME_GameSession, ConnectionInfo);
			if (!ConnectionInfo.IsEmpty())
			{
				if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
				{
					PC->ClientTravel(ConnectionInfo, TRAVEL_Absolute);
				}
			}
		}
	}
}
