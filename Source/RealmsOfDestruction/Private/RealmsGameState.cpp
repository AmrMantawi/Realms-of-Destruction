// Fill out your copyright notice in the Description page of Project Settings.


#include "RealmsGameState.h"
#include "Net/UnrealNetwork.h"

void ARealmsGameState::BeginPlay()
{
	if (GetNetMode() == ENetMode::NM_ListenServer)
	{
		MatchState = EMatchState::Character_Selection;
	}
}
void ARealmsGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARealmsGameState, MatchState);
	DOREPLIFETIME(ARealmsGameState, WinningKillCount);
}

EMatchState ARealmsGameState::GetMatchState()
{
	return MatchState;
}

void ARealmsGameState::SetMatchState(EMatchState NewMatchState)
{
	//Update match state if server attempts to change match state
	if (GetNetMode() == ENetMode::NM_ListenServer)
	{
		MatchState = NewMatchState;
		OnRep_MatchState();
	}
}

void ARealmsGameState::OnRep_MatchState()
{
	//Change in match state
	if (MatchState == EMatchState::Not_Started)
	{
		
	}
	else if (MatchState == EMatchState::Character_Selection)
	{
		//Display Character Selection UI for all players
		//Change match state to in progress after 10 sec

	}
	else if (MatchState == EMatchState::In_Progress)
	{

	}
	else if(MatchState == EMatchState::Ended)
	{
		//Decide winner
		//Display winning/losing screen
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
		    APlayerController* PlayerController = Iterator->Get();
		    //PlayerController->DisplayWinningScreen();
		}
	}
}