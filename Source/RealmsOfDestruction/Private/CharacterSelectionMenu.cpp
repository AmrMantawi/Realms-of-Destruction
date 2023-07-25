// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterSelectionMenu.h"
#include "RealmsGameState.h"

void UCharacterSelectionMenu::NativeConstruct()
{
	bCharacterSelected = false;
	UE_LOG(LogTemp, Warning, TEXT("Added character selection to player screen"));

	//Force character Selection in 10 seconds
	if (ARealmsGameState* GameState = Cast<ARealmsGameState>(GetWorld()->GetGameState()))
	{
		EMatchState MatchState = GameState->GetMatchState();
		if (MatchState == EMatchState::Character_Selection)
		{
			if (float TimerTime = GameState->GetRemainingTimerTime())
			{
				if (TimerTime > 0)
				{
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCharacterSelectionMenu::SpawnCharacter, TimerTime, false);
				}
			}
		}
	}
}

void UCharacterSelectionMenu::SetSelectedCharacter(TSubclassOf<class ACharacterMovement> Character)
{	
	//Set selected character
	SelectedCharacter = Character;
}


void UCharacterSelectionMenu::SpawnCharacter()
{
	//Set selected chatacter in player controller
	if (SelectedCharacter)
	{
		if (AGamePlayerController* PC = GetWorld()->GetFirstPlayerController<AGamePlayerController>())
		{
			PC->SelectCharacter(SelectedCharacter);
			if (ARealmsGameState* GameState = Cast<ARealmsGameState>(GetWorld()->GetGameState()))
			{
				//Spawn character and remove UI 
				EMatchState MatchState = GameState->GetMatchState();
				if (MatchState == EMatchState::Not_Started || MatchState == EMatchState::In_Progress)
				{
					PC->SpawnCharacter();
				}
			}
		}
	}
}