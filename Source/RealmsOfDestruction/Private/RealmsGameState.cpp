// Fill out your copyright notice in the Description page of Project Settings.


#include "RealmsGameState.h"
#include "Net/UnrealNetwork.h"


ARealmsGameState::ARealmsGameState()
{
    bReplicates = true;
}
void ARealmsGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ARealmsGameState, PlayersData);
}

void ARealmsGameState::Server_UpdatePlayerData_Implementation(int32 PlayerID, int32 Kills, int32 Deaths) 
{
    UE_LOG(LogTemp, Warning, TEXT("Server Update Player"));

    //Check if player in player data array 
    for (int32 i = 0; i < PlayersData.Num(); ++i)
    {
        UE_LOG(LogTemp, Warning, TEXT("Looking at player.... ID: %d|Name:%s"), PlayersData[i].PlayerID, *PlayersData[i].Name);
        UE_LOG(LogTemp, Warning, TEXT("Looking for player.... ID: %d"), PlayerID);

        if (PlayersData[i].PlayerID == PlayerID)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player Found"));
            PlayersData[i].Kills = Kills;
            PlayersData[i].Deaths = Deaths;
            UE_LOG(LogTemp, Warning, TEXT("ID:%d|Name:%s|Kills: %d|Deaths:%d"), PlayersData[i].PlayerID, *PlayersData[i].Name,Kills,Deaths);
            break;
        }
        UE_LOG(LogTemp, Warning, TEXT("Player Not Found"));

    }
}

void ARealmsGameState::Server_HandleStartingPlayer_Implementation(int32 PlayerID, const FString& PlayerName)
{
    //Check if player already in player map
    /*if (GetPlayerDataIndex(PlayerID) != NULL)*/
    for (FPlayerData& PD : PlayersData)
    {
        if (PD.PlayerID == PlayerID)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player with ID %d already exists in the player map."), PlayerID);
            return;
        }
    }

    // Create a new entry in the player map for the new player
    FPlayerData NewPlayerData;
    NewPlayerData.PlayerID = PlayerID;
    NewPlayerData.Name = PlayerName;
    NewPlayerData.Kills = 0;
    NewPlayerData.Deaths = 0;

    PlayersData.Add(NewPlayerData);

    // Debug log to show that the player has been added to the map
    UE_LOG(LogTemp, Warning, TEXT("Added player with ID %d to the player map."), PlayerID);
    UE_LOG(LogTemp, Warning, TEXT("Name %s"), *PlayerName);
}

void ARealmsGameState::Server_HandleLeavingPlayer_Implementation(int32 PlayerID)
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

TArray<FPlayerData> ARealmsGameState::GetData()
{
    return PlayersData;
}