// Fill out your copyright notice in the Description page of Project Settings.

#include "EntryJoin.h"
#include "EOSGameInstance.h"

UEntryJoin::UEntryJoin()
{
	this->OnClicked.AddDynamic(this, &UEntryJoin::JoinSession);
}

void UEntryJoin::JoinSession()
{
	UEOSGameInstance* EOSInstance = GetGameInstance<UEOSGameInstance>();
	if (EOSInstance)
	{
		if (searchResult.IsValid())
		{
			EOSInstance->JoinSessions(searchResult);
		}
	}
}