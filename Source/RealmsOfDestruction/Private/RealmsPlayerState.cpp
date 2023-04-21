// Fill out your copyright notice in the Description page of Project Settings.

#include "RealmsPlayerState.h"
#include "RealmsGameState.h"
#include "Net/UnrealNetwork.h"

ARealmsPlayerState::ARealmsPlayerState() {
	bUseCustomPlayerNames = true;
	bReplicates = true;
}
void ARealmsPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARealmsPlayerState, ID);
	DOREPLIFETIME(ARealmsPlayerState, Name);
	DOREPLIFETIME(ARealmsPlayerState, Kills);
	DOREPLIFETIME(ARealmsPlayerState, Deaths);
}

void ARealmsPlayerState::BeginPlay()
{
    Super::BeginPlay();
	if (APlayerController* PC =  GetPlayerController())
	{
		if (PC->IsLocalController())
		{
			Server_PlayerSetup(GetPlayerId(), GetPlayerName());
			//Initialize Values 
			//PlayerID = GetPlayerId();
			//DisplayName = GetPlayerName();
			//Kills = 0;
			//Deaths = 0;
			//Add To PlayerMap
		}
	}
}

//
void ARealmsPlayerState::Server_PlayerSetup_Implementation(int32 UserID, const FString& Username) {
	ID = UserID;
	Name = Username;
	if (ARealmsGameState* GameState = GetWorld()->GetGameState<ARealmsGameState>())
	{
		GameState->Server_HandleStartingPlayer_Implementation(UserID, Username);
	}
}

//Get player's kills
int32 ARealmsPlayerState::GetKills()
{
	return Kills;
}

//Record new kill
void ARealmsPlayerState::AddKill()
{
	Server_AddKill();
	UE_LOG(LogTemp, Warning, TEXT("Kills: %d"), Kills);
}

void ARealmsPlayerState::Server_AddKill_Implementation()
{
	if (ARealmsGameState* GameState = Cast<ARealmsGameState>(GetWorld()->GetGameState()))
	{
		//Increment kills
		Kills++;

		//Update data in game state
		GameState->Server_UpdatePlayerData(ID, Kills, Deaths);
	}
}

int32 ARealmsPlayerState::GetDeaths()
{
	return Deaths;
}

void ARealmsPlayerState::AddDeath()
{
	Server_AddDeath();
	UE_LOG(LogTemp, Warning, TEXT("Deaths: %d"), Deaths);
}

void ARealmsPlayerState::Server_AddDeath_Implementation()
{
	if (ARealmsGameState* GameState = Cast<ARealmsGameState>(GetWorld()->GetGameState()))
	{
		//Increment deaths
		Deaths++;

		//Update data in game state
		GameState->Server_UpdatePlayerData(ID, Kills, Deaths);
	}
}

FString ARealmsPlayerState::GetDisplayName()
{
	return Name;
}
