// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Item.generated.h"

UENUM()
enum class EItemState
{
	Active,
	Inactive
};

UCLASS()
class REALMSOFDESTRUCTION_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	bool health;
	UPROPERTY(EditAnywhere)
	bool shield;
	UPROPERTY(EditAnywhere)
	bool strength;
	UPROPERTY(EditAnywhere)
	bool speed;

	UPROPERTY(EditAnywhere)
	int healthValue;
	UPROPERTY(EditAnywhere)
	int shieldValue;
	UPROPERTY(EditAnywhere)
	int strengthValue;
	UPROPERTY(EditAnywhere)
	int speedValue;

	UPROPERTY(ReplicatedUsing = OnRep_HandleItemState)
		EItemState itemState;

	UFUNCTION()
	void OnRep_HandleItemState();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void Deactivate();
	
	UFUNCTION()
	void Activate();

	UPROPERTY(EditAnywhere)
	float respawnTime = 2.0f;

	UPROPERTY()
	FTimerHandle PickupRespawnTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

