// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"

void UHealthBar::TakeDamage(float value)
{
	DamageShield(value);
}

void UHealthBar::DamageShield(float value)
{
	float CurrentSheild = maxShield * ShieldBar->Percent;
	float Difference = (CurrentSheild - value);
	if (Difference >= 0)
	{
		ShieldBar->Percent = (Difference / maxShield);
	}
	else
	{
		DamageHealth((Difference * -1));
	}
}

void UHealthBar::DamageHealth(float value)
{
	float CurrentHealth = maxHealth * HealthBar->Percent;
	float Difference = (CurrentHealth - value);
	if (Difference >= 0)
	{
		HealthBar->Percent = (Difference / maxHealth);
	}
	else
	{
		HealthBar->Percent = 0;
		Die();
	}
}

void UHealthBar::GainHealth(float value)
{
	float CurrentHealth = maxHealth * HealthBar->Percent;
	float Addition = (CurrentHealth + value);
	if (Addition >= maxHealth)
	{
		HealthBar->Percent = 1;
	}
	else
	{
		HealthBar->Percent = (Addition / maxHealth);
	}
}

void UHealthBar::GainShield(float value)
{
	float CurrentShield = maxShield * ShieldBar->Percent;
	float Addition = (CurrentShield + value);
	if (Addition >= maxShield)
	{
		ShieldBar->Percent = 1;
	}
	else
	{
		ShieldBar->Percent = (Addition / maxShield);
	}
}

void UHealthBar::Die()
{
}
