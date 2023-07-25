// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterMovement.h"
#include "CharacterSelectButton.h"
#include "GamePlayerController.h"
#include "CharacterSelectionMenu.generated.h"

/**
 *
 */
UCLASS()
class REALMSOFDESTRUCTION_API UCharacterSelectionMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	//Set selected character
	UFUNCTION(BlueprintCallable)
	void SetSelectedCharacter(TSubclassOf<class ACharacterMovement> Character);

	//Lock in character
	UFUNCTION(BlueprintCallable)
	void SpawnCharacter();
protected:

	UFUNCTION()
	virtual void NativeConstruct() override;

	//List Of Characters
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class ACharacterMovement>> CharacterList;

	//Is character locked in
	UPROPERTY()
	bool bCharacterSelected;

private:
	//Current selected chatacter
	UPROPERTY()
	TSubclassOf<class ACharacterMovement> SelectedCharacter;

	UPROPERTY()
	FTimerHandle TimerHandle;
};
