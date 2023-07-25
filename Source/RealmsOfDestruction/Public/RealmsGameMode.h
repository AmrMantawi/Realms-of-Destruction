// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayesrDisplay.h"
#include "RealmsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API ARealmsGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:

	//Get class of players display
	UFUNCTION()
	TSubclassOf<UPlayesrDisplay> GetPlayerDisplayClass();

	//Get winnning kill count
	UFUNCTION()
	int32 GetWinningKillCount();

	//Get end game UI based on match outcome
	UFUNCTION()
	TSubclassOf<UUserWidget> GetEndGameUIClass(bool bIsWinner);


protected:
	//Number of kills required to win the game
	UPROPERTY(EditDefaultsOnly)
	int32 WinningKillCount;

	//Class of players display UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayesrDisplay> PlayerDisplayClass;

	//Winning screen UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WinningScreenUI;

	//Losing screen UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LosingScreenUI;

};
