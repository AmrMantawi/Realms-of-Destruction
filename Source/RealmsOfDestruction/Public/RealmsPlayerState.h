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

	float getKills();

	void addKill();

	FString getDisplayName();

private:
	FString displayName;
};
