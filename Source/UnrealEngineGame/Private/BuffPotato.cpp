// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffPotato.h"

ABuffPotato::ABuffPotato()
{

}

void ABuffPotato::BeginPlay()
{
	Super::BeginPlay();

	if(RightHand)
	{ 
		RightHand->OnComponentHit.AddDynamic(this, &ABuffPotato::OnHit);
	}

	if (LeftHand)
	{
		LeftHand->OnComponentHit.AddDynamic(this, &ABuffPotato::OnHit);
	}
}


void ABuffPotato::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (isPunching)
	{
		ACharacterMovement* Target = Cast<ACharacterMovement>(OtherActor);
		ACharacterMovement* Shooter = GetInstigator<ACharacterMovement>();
		if (Target && Target != Shooter && Target->GetLocalRole() == ROLE_Authority)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Player"));
			Target->CharacterTakeDamage(damage);
		}
	}
}
void ABuffPotato::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABuffPotato::StartPunch);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABuffPotato::StopPunch);

}

void ABuffPotato::StartPunch()
{
	isPunching = true;

	UE_LOG(LogTemp, Warning, TEXT("Punching"));

}

void ABuffPotato::StopPunch()
{
	isPunching = false;
}

