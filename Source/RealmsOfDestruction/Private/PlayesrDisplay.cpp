// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayesrDisplay.h"
#include "RealmsGameState.h"
#include "RealmsPlayerState.h"
#include "PlayerEntry.h"

void UPlayesrDisplay::NativeConstruct()
{
    Super::NativeConstruct();
    
    // Find the game state
    if (PlayerList && EntryClass)
    {
        PlayerList->ClearChildren();
        if (ARealmsGameState* MyGameState = GetWorld()->GetGameState<ARealmsGameState>())
        {

            // Loop through the player info map in the game state and add a row for each player
            for (auto& Entry : MyGameState->getPlayerData())
            {
                // Create a new row widget from the player row class
                UPlayerEntry* PlayerRow = CreateWidget<UPlayerEntry>(PlayerList, EntryClass);
                PlayerList->AddChild(PlayerRow);
                PlayerRow->PlayerName->SetText(FText::FromString(Entry.Value.Name));
                PlayerRow->Kills->SetText(FText::AsNumber(Entry.Value.Kills));
                PlayerRow->Deaths->SetText(FText::AsNumber(Entry.Value.Deaths));
            }
        }
    }
}
