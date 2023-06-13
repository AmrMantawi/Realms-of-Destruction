// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RealmsGameState.generated.h"

/**
 * 
 */

//Team options
UENUM()
enum class ETeam
{
    NoTeam,
    Team1,
    Team2
};

//Current state of the match
UENUM()
enum class EMatchState
{
    Not_Started,
    Character_Selection,
    In_Progress,
    Ended
};

//Data of in game player
USTRUCT(BlueprintType)
struct FPlayerData
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "PlayerData")
        int32 PlayerID = 0;

    UPROPERTY(BlueprintReadWrite, Category = "PlayerData")
        FString Name = "Unknown";

    UPROPERTY(BlueprintReadWrite, Category = "PlayerData")
        int32 Kills = 0;

    UPROPERTY(BlueprintReadWrite, Category = "PlayerData")
        int32 Deaths = 0;

    UPROPERTY(BlueprintReadWrite, Category = "PlayerData")
        ETeam Team = ETeam::NoTeam;

};

UCLASS(Abstract)
class REALMSOFDESTRUCTION_API ARealmsGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

    //Handle leaving player
    UFUNCTION(Reliable, Server)
    void Server_HandleLeavingPlayer(int32 PlayerID);
    virtual void Server_HandleLeavingPlayer_Implementation(int32 PlayerID) PURE_VIRTUAL(ARealmsGameState::Server_HandleLeavingPlayer_Implementation, );

    //Handle starting player
    UFUNCTION(Reliable, Server)
    void Server_HandleStartingPlayer(int32 PlayerID, const FString& PlayerName);
    virtual void Server_HandleStartingPlayer_Implementation(int32 playerID, const FString& PlayerName) PURE_VIRTUAL(ARealmsGameState::Server_HandleStartingPlayer_Implementation, );

    //Update player data of player with specified ID
    UFUNCTION(Reliable, Server)
    void Server_UpdatePlayerData(int32 PlayerID, int32 Kills, int32 Deaths);
    virtual void Server_UpdatePlayerData_Implementation(int32 PlayerID, int32 Kills, int32 Deaths) PURE_VIRTUAL(ARealmsGameState::Server_UpdatePlayerData_Implementation, );

    //Get data of all players in specified team
    UFUNCTION()
    virtual TArray<FPlayerData> GetData(const ETeam Team) PURE_VIRTUAL(ARealmsGameState::Server_UpdatePlayerData_Implementation, return TArray<FPlayerData>(););
    
    //Get data of player with specified ID
    UFUNCTION()
    virtual FPlayerData GetPlayerData(int32 PlayerID) PURE_VIRTUAL(ARealmsGameState::Server_UpdatePlayerData_Implementation, return FPlayerData(););

    //Start game
    UFUNCTION()
    virtual void StartGame() PURE_VIRTUAL(ARealmsGameState::StartGame, );

    //End game
    UFUNCTION()
    virtual void EndGame() PURE_VIRTUAL(ARealmsGameState::EndGame, );

    //Get match state
    UFUNCTION()
    EMatchState GetMatchState();

    //Update match state
    UFUNCTION()
    void SetMatchState(EMatchState NewMatchState);

protected: 
    virtual void BeginPlay() override;

    //Number of kills needed to win the game
    UPROPERTY(Replicated)
    int32 WinningKillCount;

    //Current state of the match
    UPROPERTY(ReplicatedUsing = OnRep_MatchState)
    EMatchState MatchState;

    // Handle logic or actions that should occur when the MatchState changes
    UFUNCTION()
    void OnRep_MatchState();

};
