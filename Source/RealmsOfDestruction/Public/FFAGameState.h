// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RealmsGameState.h"
#include "FFAGameState.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API AFFAGameState : public ARealmsGameState
{
	GENERATED_BODY()
public:
    AFFAGameState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    //Handle Leaving Player
    virtual void Server_HandleLeavingPlayer_Implementation(int32 PlayerID) override;

    //Handle Starting Player
    virtual void Server_HandleStartingPlayer_Implementation(int32 playerID, const FString& PlayerName) override;

    //Update Player Data
    virtual void Server_UpdatePlayerData_Implementation(int32 PlayerID, int32 Kills, int32 Deaths) override;

    //Get game progress data
    virtual TArray<FPlayerData> GetData(const ETeam Team) override;

    //Get data of player
    virtual FPlayerData GetPlayerData(int32 PlayerID) override;

protected:
    virtual void BeginPlay() override;

private:
    //Data of players in game
    UPROPERTY(Replicated)
    TArray<FPlayerData> PlayersData;
};
