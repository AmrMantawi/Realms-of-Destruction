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
class UNREALENGINEGAME_API UCharacterSelectionMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	//List Of Characters
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ACharacterMovement>> CharacterList;

	UFUNCTION(BlueprintCallable)
	void CharacterSelectionHelper(int characterIndex);

	/*UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "User Interface|Viewport", meta = (AdvancedDisplay = "ZOrder"))
	bool AddToPlayerScreen(int32 ZOrder = 0) override;*/

private:
	UFUNCTION()
	void SelectCharacter();


	UPROPERTY()
	TSubclassOf<class ACharacterMovement> SelectedCharacter;
};
