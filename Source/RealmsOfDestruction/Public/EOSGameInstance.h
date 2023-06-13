// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Entry.h"
#include "EntryJoin.h"
#include "EOSGameInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGameType : uint8
{
	Free_For_All,
	Team_Deathmatch,
	Custom
};

UCLASS()
class REALMSOFDESTRUCTION_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UEOSGameInstance();

	virtual void Init() override;

	//Login player
	void Login();
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	//Create a session and host it
	UFUNCTION(BlueprintCallable)
	void CreateSession(int Capacity, FString SessionName, FString GameMode, FString Map, EGameType Type);
	//UFUNCTION(BlueprintCallable)
	void CreateSession(int Capacity, FString GameMode, FString Map, EGameType Type);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	
	//Destory Session
	UFUNCTION(BlueprintCallable)
	void DestroySession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	//Find Session of specified type
	//Joins session if found and creates a session otherwise
	UFUNCTION(BlueprintCallable)
	void FindSessions(EGameType Type);
	void OnFindSessionsComplete(bool bWasSuccessful);

	//Find Available Custom Game Sessions and Display to player
	UFUNCTION()
	void FindCustomGameSessions(UPanelWidget* SessionPanel, TSubclassOf<class UEntry> EntryClassSet);
	void OnFindCustomGameSessionsComplete(bool bWasSuccessful);

	//Game Search Settings
	TSharedPtr<class FOnlineSessionSearch> SearchSettings;

	UFUNCTION(BlueprintCallable)
	void PlayersInSession();

	UPanelWidget* SessionEntryPanel;

	UPROPERTY()
	TSubclassOf<class UEntry> EntryClass;

	UPROPERTY()
	class UEntry* SessionEntry;

	int mapIndex;

	bool JoinSessions(FOnlineSessionSearchResult searchResult);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UPROPERTY(BlueprintReadOnly)
	FString PlayerUserName;

	UFUNCTION()
	bool GetLoginStatus();


protected:
	class IOnlineSubsystem* OnlineSubsystem;

	bool bIsLoggedIn;
};
