// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"


void UHealthBar::SetHealth(float value)
{
	UE_LOG(LogTemp, Warning, TEXT("Health Set: %d"), value);
	float temp = (0.365 + (0.635 * value));
	HealthBar->SetPercent(temp);

}

void UHealthBar::SetShield(float value)
{
	UE_LOG(LogTemp, Warning, TEXT("Shield Set: %d"), value);
	float temp = (0.365 + (0.41 * value));
	ShieldBar->SetPercent(temp);
}