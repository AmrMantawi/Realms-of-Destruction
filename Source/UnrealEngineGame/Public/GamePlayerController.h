// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterMovement.h"
#include "GamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALENGINEGAME_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
		 
public:

	//Select Character
	void SelectCharacter(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint);

	UFUNCTION(Reliable, Server)
	void Server_SelectCharacter(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint);
	void Server_SelectCharacter_Implementation(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint);

	//Spawn Character and Possess
	void SpawnCharacter();

	UFUNCTION(Reliable, Server)
	void Server_SpawnCharacter(FVector Location, FRotator Rotation);
	void Server_SpawnCharacter_Implementation(FVector Location, FRotator Rotation);

	void Die();

private:

	UPROPERTY()
	TSubclassOf<class ACharacterMovement> SelectedCharacter;

	ACharacterMovement* currentCharacter;
};
