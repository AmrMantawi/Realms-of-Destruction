// Fill out your copyright notice in the Description page of Project Settings.

#include "RealmsPlayerState.h"
#include "RealmsGameState.h"

ARealmsPlayerState::ARealmsPlayerState() {
	bUseCustomPlayerNames = true;
}
void ARealmsPlayerState::BeginPlay()
{
    Super::BeginPlay();
	displayName = GetPlayerName();
	SetScore(0);
	UE_LOG(LogTemp, Warning, TEXT("Player stat begin"));

	if (ARealmsGameState* gameState = Cast<ARealmsGameState>(GetWorld()->GetGameState()))
	{
		UE_LOG(LogTemp, Warning, TEXT("worked"));

		gameState->HandleStartingPlayer(GetPlayerId(), GetPlayerName());
	}
}

float ARealmsPlayerState::getKills()
{
	return GetScore();
}
void ARealmsPlayerState::addKill()
{
	SetScore(GetScore() + 1);
	if (ARealmsGameState* gameState = Cast<ARealmsGameState>(GetWorld()->GetGameState()))
	{
		gameState->UpdatePlayerData(GetPlayerId(), Score, 0);
	}
	UE_LOG(LogTemp, Warning, TEXT("Kills: %f"), GetScore());
}
FString ARealmsPlayerState::getDisplayName()
{
	return displayName;
}
