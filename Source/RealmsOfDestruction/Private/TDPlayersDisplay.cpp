// Fill out your copyright notice in the Description page of Project Settings.


#include "TDPlayersDisplay.h"
#include "RealmsPlayerState.h"
#include "PlayerEntry.h"
#include "TeamDeathmatchGameState.h"

void UTDPlayersDisplay::NativeConstruct()
{
    Super::NativeConstruct();

    // Find the game state
    if (Team1 && Team2 && EntryClass)
    {
        Team1->ClearChildren();
        Team2->ClearChildren();
        if (ARealmsGameState* MyGameState = GetWorld()->GetGameState<ATeamDeathmatchGameState>())
        {
            // Loop through the player info map in the game state and add a row for each player
            for (auto& Entry : MyGameState->GetData(ETeam::Team1))
            {
                UE_LOG(LogTemp, Warning, TEXT("Name: %s\nKills: %d\nDeaths: %d"), &Entry.Name, Entry.Kills, Entry.Deaths);
                // Create a new row widget from the player row class
                UPlayerEntry* PlayerRow = CreateWidget<UPlayerEntry>(Team1, EntryClass);
                Team1->AddChild(PlayerRow);
                PlayerRow->PlayerName->SetText(FText::FromString(Entry.Name));
                PlayerRow->Kills->SetText(FText::AsNumber(Entry.Kills));
                PlayerRow->Deaths->SetText(FText::AsNumber(Entry.Deaths));
            }
            for (auto& Entry : MyGameState->GetData(ETeam::Team2))
            {
                UE_LOG(LogTemp, Warning, TEXT("Name: %s\nKills: %d\nDeaths: %d"), &Entry.Name, Entry.Kills, Entry.Deaths);
                // Create a new row widget from the player row class
                UPlayerEntry* PlayerRow = CreateWidget<UPlayerEntry>(Team2, EntryClass);
                Team2->AddChild(PlayerRow);
                PlayerRow->PlayerName->SetText(FText::FromString(Entry.Name));
                PlayerRow->Kills->SetText(FText::AsNumber(Entry.Kills));
                PlayerRow->Deaths->SetText(FText::AsNumber(Entry.Deaths));
            }
        }
    }
}