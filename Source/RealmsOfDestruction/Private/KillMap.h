// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class KillMap
{
public:
	KillMap();
	KillMap(int size, FString* players);
	~KillMap();

	UFUNCTION(BlueprintCallable)
	void AddKill(const FString& PlayerName);
	
	UFUNCTION(BlueprintCallable)
	int32 GetKills(const FString& PlayerName);

	UFUNCTION(BlueprintCallable)
	void Clear();



private:
	TMap<FString, int32> KillsMap;
};
