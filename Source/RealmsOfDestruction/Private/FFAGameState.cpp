// Fill out your copyright notice in the Description page of Project Settings.


#include "FFAGameState.h"
#include "RealmsGameMode.h"
#include "Net/UnrealNetwork.h"
AFFAGameState::AFFAGameState()
{
    bReplicates = true;
}

void AFFAGameState::BeginPlay()
{
    if (GetWorld() && GetWorld()->IsServer())
    {
        if (ARealmsGameMode* GameMode = Cast<ARealmsGameMode>(GetWorld()->GetAuthGameMode()))
        {
            WinningKillCount = GameMode->GetWinningKillCount();
        }
    }
}

void AFFAGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFFAGameState, PlayersData);
}

void AFFAGameState::Server_UpdatePlayerData_Implementation(int32 PlayerID, int32 Kills, int32 Deaths)
{
    UE_LOG(LogTemp, Warning, TEXT("FFA Update Player Data"));
    //Check if player in player data array 
    for (int32 i = 0; i < PlayersData.Num(); ++i)
    {
        if (PlayersData[i].PlayerID == PlayerID)
        {
            PlayersData[i].Kills = Kills;
            PlayersData[i].Deaths = Deaths;
            break;
        }
    }
       
    //End game if player reached winning kill count
    if (Kills >= WinningKillCount)
    {

    }
}

void AFFAGameState::Server_HandleStartingPlayer_Implementation(int32 PlayerID, const FString& PlayerName)
{
    //Check if player already in player map
    for (FPlayerData& PD : PlayersData)
    {
        if (PD.PlayerID == PlayerID)
        {
            return;
        }
    }

    // Create a new entry in the player map for the new player
    FPlayerData NewPlayerData;
    NewPlayerData.PlayerID = PlayerID;
    NewPlayerData.Name = PlayerName;
    NewPlayerData.Kills = 0;
    NewPlayerData.Deaths = 0;
    NewPlayerData.Team = ETeam::NoTeam;

    PlayersData.Add(NewPlayerData);

    // Debug log to show that the player has been added to the map
    UE_LOG(LogTemp, Warning, TEXT("Added player with ID %d to the player map."), PlayerID);
    UE_LOG(LogTemp, Warning, TEXT("Name %s"), *PlayerName);
}

void AFFAGameState::Server_HandleLeavingPlayer_Implementation(int32 PlayerID)
{
    //Remove player's data if found in players data array
    for (int32 i = 0; i < PlayersData.Num(); ++i)
    {
        if (PlayersData[i].PlayerID == PlayerID)
        {
            PlayersData.RemoveAt(i);
            return;
        }
    }
}

TArray<FPlayerData> AFFAGameState::GetData(const ETeam Team)
{
    //Retrun Data of all players
    return PlayersData;
}

FPlayerData AFFAGameState::GetPlayerData(int32 PlayerID)
{
    //Get Player Data
    for (FPlayerData& item : PlayersData)
    {
        if (item.PlayerID == PlayerID)
            return item;
    }
    return FPlayerData();
}