// Fill out your copyright notice in the Description page of Project Settings.


#include "Bot.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

ABot::ABot()
{
	bReplicates = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}


void ABot::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABot, FlySpeed);
}

void ABot::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABot::Fall);
}

// Called every frame
void ABot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Apply flying force
}

// Fly upwards
void ABot::StartJump()
{
	AddMovementInput(FVector(0, 0, 1), 1000.0f);
}

// Fly downwards
void ABot::StopJump()
{

}

void ABot::StartSprint()
{
	GetCharacterMovement()->MaxFlySpeed *= speedMultiplier;
	Server_StartSprint();
}

void ABot::StopSprint()
{
	GetCharacterMovement()->MaxFlySpeed /= speedMultiplier;
	Server_StopSprint();
}

void ABot::Server_StartSprint_Implementation()
{
	GetCharacterMovement()->MaxFlySpeed *= speedMultiplier;
}

void ABot::Server_StopSprint_Implementation()
{
	GetCharacterMovement()->MaxFlySpeed /= speedMultiplier;
}

void ABot::Fall()
{
	AddMovementInput(FVector(0, 0, 1), -600.0f);
}