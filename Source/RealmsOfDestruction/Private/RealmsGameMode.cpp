// Fill out your copyright notice in the Description page of Project Settings.


#include "RealmsGameMode.h"
#include "RealmsGameState.h"

TSubclassOf<UPlayesrDisplay> ARealmsGameMode::GetPlayerDisplayClass()
{
	return PlayerDisplayClass;
}

int32 ARealmsGameMode::GetWinningKillCount()
{
	return WinningKillCount;
}