// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class UNREALENGINEGAME_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealth(float value);
	void SetShield(float value);

private:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UProgressBar* ShieldBar;
};
