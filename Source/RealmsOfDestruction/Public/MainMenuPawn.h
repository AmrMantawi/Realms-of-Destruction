// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UFUNCTION()
	void TogglePauseMenu();

	UPROPERTY()
	class UMainMenu* PlayerMainMenu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainMenu> MainMenuClass;

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
