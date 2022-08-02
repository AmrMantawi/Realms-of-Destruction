// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectionPawn.h"

// Sets default values
ACharacterSelectionPawn::ACharacterSelectionPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterSelectionPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterSelectionPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterSelectionPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

