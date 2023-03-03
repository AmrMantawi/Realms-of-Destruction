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
   // virtual void BeginPlay() override;
	void UpdatePlayerData(int32 PlayerID, int32 kills, int32 deaths);

    void HandleStartingPlayer(int32 PlayerID, FString PlayerName);

    void HandleLeavingPlayer(int32 PlayeID);

    TMap<int32, FPlayerData> getPlayerData();

private:
    UPROPERTY()
    TMap<int32, FPlayerData> PlayerMap;

};
