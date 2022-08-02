// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterSelectionMenu.h"

void UCharacterSelectionMenu::CharacterSelectionHelper(int characterIndex)
{
	SelectedCharacter = CharacterList[characterIndex];
	SelectCharacter();
}


void UCharacterSelectionMenu::SelectCharacter()
{
	
	if (APawn* PlayerPawn = GetOwningPlayerPawn())
	{
		if (AGamePlayerController* PC = GetWorld()->GetFirstPlayerController<AGamePlayerController>())
		{
			PC->SelectCharacter(SelectedCharacter);
			PC->SpawnCharacter();

			this->SetVisibility(ESlateVisibility::Hidden);

			/*
			//controller->UnPossess();
			//APawn* character = GetWorld()->SpawnActor<APawn>(SelectedCharacter);
			//controller->AController::Possess(character);
			//PlayerPawn->Destroy();
			if (controller)
			{
				FInputModeGameOnly InputModeData;
				controller->SetInputMode(InputModeData);
				controller->SetShowMouseCursor(false);
				this->SetVisibility(ESlateVisibility::Hidden);
			}
			*/
		}
	}
}