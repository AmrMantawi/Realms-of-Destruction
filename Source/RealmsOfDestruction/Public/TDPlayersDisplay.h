// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayesrDisplay.h"
#include "TDPlayersDisplay.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API UTDPlayersDisplay : public UPlayesrDisplay
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerEntry> EntryClass;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* Team1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* Team2;
};
