// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Resume;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChangeCharacter;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Quit;
};
