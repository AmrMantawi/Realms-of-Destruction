// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "PlayerEntry.h"
#include "PlayesrDisplay.generated.h"

/**
 * 
 */

UCLASS()
class REALMSOFDESTRUCTION_API UPlayesrDisplay : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	void UpdateUI();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerEntry> EntryClass;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* PlayerList;


};
