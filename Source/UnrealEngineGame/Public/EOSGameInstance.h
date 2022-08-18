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
#include "EOSGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class UNREALENGINEGAME_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UEOSGameInstance();

	virtual void Init() override;

	void Login();
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	UFUNCTION(BlueprintCallable)
	void CreateSession();
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void DestroySession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION()
	void FindSessions(UPanelWidget* SessionPanel, TSubclassOf<class UEntry> EntryClassSet);
	void OnFindSessionsComplete(bool bWasSuccessful);
	TSharedPtr<class FOnlineSessionSearch> SearchSettings;

	UPanelWidget* SessionEntryPanel;

	UPROPERTY()
	TSubclassOf<class UEntry> EntryClass;

	UPROPERTY()
	class UEntry* SessionEntry;

	int SessionIndex;

	UFUNCTION()
	void JoinSessions();
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UPROPERTY(BlueprintReadOnly)
	FString PlayerUserName;

protected:
	class IOnlineSubsystem* OnlineSubsystem;

	bool bIsLoggedIn;
};
