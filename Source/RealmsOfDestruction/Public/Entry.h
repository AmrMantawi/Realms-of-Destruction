// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EntryJoin.h"
#include "Entry.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API UEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTextBlock* SessionName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTextBlock* HostName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTextBlock* Capacity;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTextBlock* Description;


	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UEntryJoin* JoinButton;
};
