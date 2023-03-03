// Fill out your copyright notice in the Description page of Project Settings.


#include "KillMap.h"

KillMap::KillMap(int size, FString* players)
{
	KillsMap = TMap<FString, int32>();
}

void KillMap::AddKill(const FString& PlayerName)
{
    if (KillsMap.Contains(PlayerName))
    {
        KillsMap[PlayerName]++;
    }
    else
    {
        KillsMap.Add(PlayerName, 1);
    }
}

int32 KillMap::GetKills(const FString& PlayerName)
{
    const int32* KillsPtr = KillsMap.Find(PlayerName);
    return (KillsPtr != nullptr) ? *KillsPtr : 0;
}

void KillMap::Clear()
{
    KillsMap.Empty();
}

KillMap::~KillMap()
{
    // Free any dynamically allocated memory
    for (auto& Pair : KillsMap)
    {
        FString PlayerName = Pair.Key;
        int32* KillsPtr = &Pair.Value;
        delete KillsPtr;
    }

    delete &KillsMap;
}
