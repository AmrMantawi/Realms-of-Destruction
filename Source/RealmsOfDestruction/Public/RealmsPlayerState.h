// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RealmsPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API ARealmsPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ARealmsPlayerState();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_PlayerSetup(int32 UserID, const FString& Username);
	void Server_PlayerSetup_Implementation(int32 UserID, const FString& Username);

	int32 GetKills();

	void AddKill();

	UFUNCTION(Server, Reliable)
	void Server_AddKill();
	void Server_AddKill_Implementation();

	int32 GetDeaths();

	void AddDeath();

	UFUNCTION(Server, Reliable)
	void Server_AddDeath();
	void Server_AddDeath_Implementation();

	FString GetDisplayName();




private:
	UPROPERTY(Replicated)
	int32 ID;

	UPROPERTY(Replicated)
	FString Name;

	UPROPERTY(Replicated)
	int32 Kills;

	UPROPERTY(Replicated)
	int32 Deaths;
};
