// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Entry.generated.h"

/**
 * 
 */
UCLASS()
class UNREALENGINEGAME_API UEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTextBlock* Name;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTextBlock* Capacity;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* JoinButton;
};
