// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenu.h"
#include "MainMenu.h"
#include "GameFramework/Pawn.h"
#include "MainMenuPawn.generated.h"

UCLASS()
class REALMSOFDESTRUCTION_API AMainMenuPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainMenuPawn();
	UPROPERTY()
	class USettingsMenu* playerSettingsMenu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class USettingsMenu> SettingsMenuClass;

	UPROPERTY()
	bool settingsDisplayed = false;

	UFUNCTION()
	void ToggleSettingsMenu();

	UPROPERTY()
	class UMainMenu* playerMainMenu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainMenu> MainMenuClass;

	//UPROPERTY()
	//class APlayerController* playerController;

	APlayerController* PC;

	virtual void PossessedBy(AController* NewController) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
