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
class REALMSOFDESTRUCTION_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
		 
public:

	AGamePlayerController();

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	//Setup Player Controller 
	UFUNCTION(Client, Reliable)
	void Client_Setup(TSubclassOf<class UPlayesrDisplay> PlayersDisplayClass);
	void Client_Setup_Implementation(TSubclassOf<class UPlayesrDisplay> PlayersDisplayClass);

	UFUNCTION(Server, Reliable)
	void Server_GetPlayerDisplayClass();
	void Server_GetPlayerDisplayClass_Implementation();

	//Select Character
	UFUNCTION()
	void SelectCharacter(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint);
	UFUNCTION(Reliable, Server)
	void Server_SelectCharacter(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint);
	void Server_SelectCharacter_Implementation(TSubclassOf<ACharacterMovement> SelectedCharacterBlueprint);

	//Spawn Character and Possess it
	UFUNCTION()
	void SpawnCharacter();
	UFUNCTION(Reliable, Server)
	void Server_SpawnCharacter(FVector Location, FRotator Rotation);
	void Server_SpawnCharacter_Implementation(FVector Location, FRotator Rotation);

	//Handle Player Death
	UFUNCTION()
	void Die();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	//Current selected character
	UPROPERTY(Replicated)
	ACharacterMovement* currentCharacter;

	//Toggle player display UI
	UFUNCTION()
	void TogglePlayerDisplay();

	UPROPERTY()
	bool bPlayersMenuDisplayed = false;

	//Player Display UI
	UPROPERTY()
	class UPlayesrDisplay* PlayersDisplay;

private:

	UPROPERTY()
	TSubclassOf<class ACharacterMovement> SelectedCharacter;


};
