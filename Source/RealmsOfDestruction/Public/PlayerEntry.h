// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "PlayerEntry.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API UPlayerEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Kills;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Deaths;
	
};
