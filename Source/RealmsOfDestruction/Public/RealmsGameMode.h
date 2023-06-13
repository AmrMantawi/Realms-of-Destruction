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
	TSubclassOf<UPlayesrDisplay> GetPlayerDisplayClass();

	//Class of players display UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayesrDisplay> PlayerDisplayClass;

	//Get winnning kill count
	UFUNCTION()
	int32 GetWinningKillCount();

protected:
	//Number of kills required to win the game
	UPROPERTY(EditDefaultsOnly)
	int32 WinningKillCount;

};
