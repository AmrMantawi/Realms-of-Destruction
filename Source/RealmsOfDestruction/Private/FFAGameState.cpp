// Fill out your copyright notice in the Description page of Project Settings.


#include "FFAGameState.h"
#include "RealmsGameMode.h"
#include "RealmsPlayerState.h"
#include "GamePlayerController.h"
#include "Net/UnrealNetwork.h"
AFFAGameState::AFFAGameState()
{
    bReplicates = true;
}

void AFFAGameState::BeginPlay()
{
    if (IsNetMode(ENetMode::NM_ListenServer) || IsNetMode(ENetMode::NM_DedicatedServer))
    {
        if (ARealmsGameMode* GameMode = Cast<ARealmsGameMode>(GetWorld()->GetAuthGameMode()))
        {
            WinningKillCount = GameMode->GetWinningKillCount();
        }
    }

    SetMatchState(EMatchState::Not_Started);
}

void AFFAGameState::CharacterSelect()
{
    if (IsNetMode(ENetMode::NM_ListenServer) || IsNetMode(ENetMode::NM_DedicatedServer))
    {
        UE_LOG(LogTemp, Warning, TEXT("Character Selection"));


        //Set match state to character selection
        SetMatchState(EMatchState::Character_Selection);

        //Set a timer for character selection
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFFAGameState::StartGame, 10.0f, false);
    }
}

void AFFAGameState::StartGame()
{
    UE_LOG(LogTemp, Warning, TEXT("Game Started"));
    //Set match state to in progress
    SetMatchState(EMatchState::In_Progress);
}

void AFFAGameState::Multicast_EndGame_Implementation(int32 WinningPlayerID)
{
    //Check if in winning player world
    if (AGamePlayerController* PC = GetWorld()->GetFirstPlayerController<AGamePlayerController>())
    {
        if (APlayerState* PS = PC->GetPlayerState<APlayerState>())
        {
            PC->DisplayEndGameScreen(PS->GetPlayerId() == WinningPlayerID);
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
    //Return if match is not active
    if (MatchState != EMatchState::In_Progress)
        return;

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
        //Update Match State
        SetMatchState(EMatchState::Ended);
        Multicast_EndGame(PlayerID);
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

    //CharacterSelect();
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
    for (FPlayerData& Player : PlayersData)
    {
        if (Player.PlayerID == PlayerID)
            return Player;
    }
    return FPlayerData();
}

void AFFAGameState::OnRep_MatchState()
{
    if (AGamePlayerController* PC = GetWorld()->GetFirstPlayerController<AGamePlayerController>())
    {
        if (MatchState == EMatchState::Not_Started)
        {
            //Spawn player in lobby
            UE_LOG(LogTemp, Warning, TEXT("Not started state"));
        }
        else if (MatchState == EMatchState::Character_Selection)
        {
            //Spawn character selection pawn
            PC->Client_CharacterSelection();
            UE_LOG(LogTemp, Warning, TEXT("Character selection state"));

        }
        else if (MatchState == EMatchState::In_Progress)
        {
            //Spawn selected character in
            PC->SpawnCharacter();

            UE_LOG(LogTemp, Warning, TEXT("In Progress state"));

        }
        else if (MatchState == EMatchState::Ended)
        {
            //Take player back to main menu

            UE_LOG(LogTemp, Warning, TEXT("Ended state"));

        }
    }
}