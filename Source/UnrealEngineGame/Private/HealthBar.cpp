// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"


void UHealthBar::SetHealth(float value)
{
	HealthBar->Percent = value;
}

void UHealthBar::SetShield(float value)
{
	ShieldBar->Percent = value;
}