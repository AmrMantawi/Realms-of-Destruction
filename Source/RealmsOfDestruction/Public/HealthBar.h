// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetHealth(float Value);
	
	UFUNCTION()
	void SetShield(float Value);

	UFUNCTION()
	void ShowSpeed();

	UFUNCTION()
	void ShowStrength();

	UFUNCTION()
	void HideSpeed();

	UFUNCTION()
	void HideStrength();


private:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UProgressBar* ShieldBar;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UImage* Speed;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UImage* Strength;
};
