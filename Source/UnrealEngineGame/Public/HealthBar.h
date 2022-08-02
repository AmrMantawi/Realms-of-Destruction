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

	//Take Damage
	void TakeDamage(float value);

	//Damage To Health
	void DamageHealth(float value);

	//Damage to Sheild
	void DamageShield(float value);

	//Gain Health
	void GainHealth(float value);

	//Gain Sheild
	void GainShield(float value);

private:

	void Die();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UProgressBar* ShieldBar;

	UPROPERTY(EditAnywhere)
		float maxHealth;

	UPROPERTY(EditAnywhere)
		float maxShield;
};
