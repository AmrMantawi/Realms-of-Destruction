// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterMovement.h"
#include "Components/BoxComponent.h"
#include "RealmsPlayerState.h"
#include "BuffPotato.generated.h"

/**
 * 
 */
UCLASS()
class REALMSOFDESTRUCTION_API ABuffPotato : public ACharacterMovement
{
	GENERATED_BODY()
	
public:
	ABuffPotato();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


	UPROPERTY(replicated, BlueprintReadOnly)
	bool isPunching = false;

	UPROPERTY(EditAnywhere, Category = Hand)
	UBoxComponent* RightHand;

	UPROPERTY(EditAnywhere, Category = Hand)
	UBoxComponent* LeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Punch")
	float PunchForce = 1000.0f;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Server, Reliable)
	void Server_StartPunch();
	void Server_StartPunch_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_StopPunch();
	void Server_StopPunch_Implementation();

	UPROPERTY(EditAnywhere, Category = "Punch")
	float damage = 30.0f;
};
