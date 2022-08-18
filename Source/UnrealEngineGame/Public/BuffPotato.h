// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterMovement.h"
#include "Components/SphereComponent.h"
#include "BuffPotato.generated.h"

/**
 * 
 */
UCLASS()
class UNREALENGINEGAME_API ABuffPotato : public ACharacterMovement
{
	GENERATED_BODY()
	
public:
	ABuffPotato();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	bool isPunching = false;

	UPROPERTY(EditDefaultsOnly, Category = Hand)
	USphereComponent* RightHand;

	UPROPERTY(EditDefaultsOnly, Category = Hand)
	USphereComponent* LeftHand;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void StartPunch();

	UFUNCTION()
	void StopPunch();

	UPROPERTY(EditAnywhere)
	float damage;
};
