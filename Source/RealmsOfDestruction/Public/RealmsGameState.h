// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RealmsGameState.generated.h"

/**
 * 
 */
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
};

UCLASS()
class REALMSOFDESTRUCTION_API ARealmsGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
    ARealmsGameState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(Reliable, Server)
    void Server_HandleLeavingPlayer(int32 PlayerID);
    void Server_HandleLeavingPlayer_Implementation(int32 PlayerID);

    UFUNCTION(Reliable, Server)
    void Server_HandleStartingPlayer(int32 PlayerID, const FString& PlayerName);
    void Server_HandleStartingPlayer_Implementation(int32 playerID, const FString& PlayerName);

    UFUNCTION(Reliable, Server)
    void Server_UpdatePlayerData(int32 PlayerID, int32 Kills, int32 Deaths);
    void Server_UpdatePlayerData_Implementation(int32 PlayerID, int32 Kills, int32 Deaths);

    UFUNCTION()
    TArray<FPlayerData> GetData();

//private:
    UPROPERTY(Replicated)
    TArray<FPlayerData> PlayersData;

};
