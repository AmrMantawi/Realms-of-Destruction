// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterSelectionMenu.h"

void UCharacterSelectionMenu::CharacterSelectionHelper(int characterIndex)
{
	if(CharacterList[characterIndex])
	{
		SelectedCharacter = CharacterList[characterIndex];
		SelectCharacter();
	}
}


void UCharacterSelectionMenu::SelectCharacter()
{
	
	if (APawn* PlayerPawn = GetOwningPlayerPawn())
	{
		if (AGamePlayerController* PC = GetWorld()->GetFirstPlayerController<AGamePlayerController>())
		{
			PC->SelectCharacter(SelectedCharacter);
		}
	}
}

void UCharacterSelectionMenu::SpawnCharacter()
{
	if (APawn* PlayerPawn = GetOwningPlayerPawn())
	{
		if (AGamePlayerController* PC = GetWorld()->GetFirstPlayerController<AGamePlayerController>())
		{
			PC->SpawnCharacter();
			this->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}