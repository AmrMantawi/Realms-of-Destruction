// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
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

	UPROPERTY(EditDefaultsOnly)
	bool bHealthBoost;
	UPROPERTY(EditDefaultsOnly)
	bool bShieldBoost;
	UPROPERTY(EditDefaultsOnly)
	bool bStrengthBoost;
	UPROPERTY(EditDefaultsOnly)
	bool bSpeedBoost;

	UPROPERTY(EditDefaultsOnly)
	int HealthValue;
	UPROPERTY(EditDefaultsOnly)
	int ShieldValue;
	UPROPERTY(EditDefaultsOnly)
	int StrengthValue;
	UPROPERTY(EditDefaultsOnly)
	int SpeedValue;

	UPROPERTY(ReplicatedUsing = OnRep_HandleItemState)
	EItemState ItemState;

	UFUNCTION()
	void OnRep_HandleItemState();

	//Called on overlap with another component
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void Deactivate();
	
	UFUNCTION()
	void Activate();

	UPROPERTY()
	FTimerHandle PickupRespawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UNiagaraSystem* NiagraSystem;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float RespawnTime = 2.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

