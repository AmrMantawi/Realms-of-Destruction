// Fill out your copyright notice in the Description page of Project Settings.


#include "RealmsGameMode.h"
#include "RealmsGameState.h"

TSubclassOf<UPlayesrDisplay> ARealmsGameMode::GetPlayerDisplayClass()
{
	return PlayerDisplayClass;
}

TSubclassOf<UUserWidget> ARealmsGameMode::GetEndGameUIClass(bool bIsWinner)
{
	//Return winning screen if winner
	if (bIsWinner)
	{
		return WinningScreenUI;
	}
	//Return losing screen if looser
	else {
		return LosingScreenUI;
	}
}

int32 ARealmsGameMode::GetWinningKillCount()
{
	return WinningKillCount;
}