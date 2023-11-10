// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"


void UHealthBar::SetHealth(float Value)
{
	//Update value of health bar
	float temp = (0.365 + (0.635 * Value));
	HealthBar->SetPercent(temp);
}

void UHealthBar::SetShield(float Value)
{
	//Update value of shield bar
	float temp = (0.365 + (0.41 * Value));
	ShieldBar->SetPercent(temp);
}

void UHealthBar::ShowSpeed()
{
	Speed->SetVisibility(ESlateVisibility::Visible);
}

void UHealthBar::ShowStrength()
{
	Strength->SetVisibility(ESlateVisibility::Visible);

}

void UHealthBar::HideSpeed()
{
	Speed->SetVisibility(ESlateVisibility::Hidden);

}

void UHealthBar::HideStrength()
{
	Strength->SetVisibility(ESlateVisibility::Hidden);

}