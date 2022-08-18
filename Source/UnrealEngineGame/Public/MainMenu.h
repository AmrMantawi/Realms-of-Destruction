// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "EOSGameInstance.h"
#include "Entry.h"
#include "MainMenu.generated.h"

/**
 * 
 */

UCLASS()
class UNREALENGINEGAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void MenuSetup();

	UFUNCTION(BlueprintCallable)
	void DisplaySessions(UPanelWidget* SessionEntryPanel);

	void JoinUserSession();

	int SessionIndex;

	class UEOSGameInstance* EOSInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UEntry> EntryClass;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTextBlock* UserNameText;

	UPROPERTY()
	class UEntry* SessionEntry;
private:
	void MenuTearDown();

protected:
	class IOnlineSubsystem* OnlineSubsystem;

	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
};
