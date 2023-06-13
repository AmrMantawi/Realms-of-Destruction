// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayesrDisplay.h"
#include "FFAPlayersDisplay.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API UFFAPlayersDisplay : public UPlayesrDisplay
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerEntry> EntryClass;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* PlayerList;
};
