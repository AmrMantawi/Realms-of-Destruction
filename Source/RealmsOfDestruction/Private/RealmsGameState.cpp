// Fill out your copyright notice in the Description page of Project Settings.


#include "RealmsGameState.h"
void ARealmsGameState::UpdatePlayerData(int32 PlayerID, int32 kills, int32 deaths){
    if (PlayerMap.Contains(PlayerID))
    {
        FPlayerData CurrentPlayerData = PlayerMap[PlayerID];
        CurrentPlayerData.Kills = kills;
        CurrentPlayerData.Deaths = deaths;

        // Overwrite the entry in the player map with the updated player data
        PlayerMap[PlayerID] = CurrentPlayerData;
    }
}
void ARealmsGameState::HandleStartingPlayer(int32 PlayerID, FString PlayerName)
{
    if (PlayerMap.Contains(PlayerID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Player with ID %d already exists in the player map."), PlayerID);
        return;
    }
    // Create a new entry in the player map for the new player
    FPlayerData NewPlayerData;
    NewPlayerData.Name = PlayerName;
    NewPlayerData.Kills = 0;
    NewPlayerData.Deaths = 0;

    PlayerMap.Add(PlayerID, NewPlayerData);

    // Debug log to show that the player has been added to the map
    UE_LOG(LogTemp, Display, TEXT("Added player with ID %d to the player map."), PlayerID);
}


TMap<int32, FPlayerData> ARealmsGameState::getPlayerData()
{
    return PlayerMap;
}
