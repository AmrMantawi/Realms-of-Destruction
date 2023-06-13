// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamDeathmatchGameState.h"
#include "RealmsPlayerState.h"
#include "Net/UnrealNetwork.h"
ATeamDeathmatchGameState::ATeamDeathmatchGameState()
{
    bReplicates = true;
}

void ATeamDeathmatchGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATeamDeathmatchGameState, Team1Data);
    DOREPLIFETIME(ATeamDeathmatchGameState, Team2Data);
}
void ATeamDeathmatchGameState::Server_UpdatePlayerData_Implementation(int32 PlayerID, int32 Kills, int32 Deaths)
{
    UE_LOG(LogTemp, Warning, TEXT("FFA Update Player Data"));

    //Check if player in player data array 
    for (int32 i = 0; i < Team1Data.Num(); ++i)
    {
        if (Team1Data[i].PlayerID == PlayerID)
        {
            Team1Data[i].Kills = Kills;
            Team1Data[i].Deaths = Deaths;
            break;
        }
    }

    for (int32 i = 0; i < Team2Data.Num(); ++i)
    {
        if (Team2Data[i].PlayerID == PlayerID)
        {
            Team2Data[i].Kills = Kills;
            Team2Data[i].Deaths = Deaths;
            break;
        }
    }
}

void ATeamDeathmatchGameState::Server_HandleStartingPlayer_Implementation(int32 PlayerID, const FString& PlayerName)
{
    //Check if player already in player map
    for (FPlayerData& PD : Team1Data)
    {
        if (PD.PlayerID == PlayerID)
        {
            return;
        }
    }
    for (FPlayerData& PD : Team2Data)
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

    if (Team1Data.Num() <= Team2Data.Num())
    {
        NewPlayerData.Team = ETeam::Team1;
        Team1Data.Add(NewPlayerData);
    }
    else
    {
        NewPlayerData.Team = ETeam::Team2;
        Team2Data.Add(NewPlayerData);
    }

    // Debug log to show that the player has been added to the map
    UE_LOG(LogTemp, Warning, TEXT("Added player with ID %d to the player map."), PlayerID);
    UE_LOG(LogTemp, Warning, TEXT("Name %s"), *PlayerName);
}

void ATeamDeathmatchGameState::Server_HandleLeavingPlayer_Implementation(int32 PlayerID)
{
    //Remove player's data if found in players data array
    for (int32 i = 0; i < Team1Data.Num(); ++i)
    {
        if (Team1Data[i].PlayerID == PlayerID)
        {
            Team1Data.RemoveAt(i);
            return;
        }
    }
    //Remove player's data if found in players data array
    for (int32 i = 0; i < Team2Data.Num(); ++i)
    {
        if (Team2Data[i].PlayerID == PlayerID)
        {
            Team2Data.RemoveAt(i);
            return;
        }
    }
}

TArray<FPlayerData> ATeamDeathmatchGameState::GetData(const ETeam Team)
{
    //Get Team Data
    if (Team == ETeam::Team1)
        return Team1Data;
    else if (Team == ETeam::Team2)
        return Team2Data;
    else
    {
        //Get all players' data
        TArray<FPlayerData> ConcatenatedData;
        ConcatenatedData.Append(Team1Data);
        ConcatenatedData.Append(Team2Data);
        return ConcatenatedData;
    }
}

FPlayerData ATeamDeathmatchGameState::GetPlayerData(int32 PlayerID)
{
    for (FPlayerData& PD : Team1Data)
    {
        if (PD.PlayerID == PlayerID)
        {
            return PD;
        }
    }

    for (FPlayerData& PD : Team2Data)
    {
        if (PD.PlayerID == PlayerID)
        {
            return PD;
        }
    }

    return FPlayerData();
}