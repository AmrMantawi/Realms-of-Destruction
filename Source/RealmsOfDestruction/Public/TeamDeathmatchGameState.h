// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RealmsGameState.h"
#include "TeamDeathmatchGameState.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API ATeamDeathmatchGameState : public ARealmsGameState
{
	GENERATED_BODY()
public:
    ATeamDeathmatchGameState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void Server_HandleLeavingPlayer_Implementation(int32 PlayerID) override;

    virtual void Server_HandleStartingPlayer_Implementation(int32 playerID, const FString& PlayerName) override;

    virtual void Server_UpdatePlayerData_Implementation(int32 PlayerID, int32 Kills, int32 Deaths) override;

    virtual TArray<FPlayerData> GetData(const ETeam Team) override;

    virtual FPlayerData GetPlayerData(int32 PlayerID) override;

private:
    UPROPERTY(Replicated)
    TArray<FPlayerData> Team1Data;

    UPROPERTY(Replicated)
    TArray<FPlayerData> Team2Data;
	
};
