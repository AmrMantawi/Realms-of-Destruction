// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/SkeletalMesh.h"
#include "ShowCharacter.generated.h"

UCLASS()
class UNREALENGINEGAME_API AShowCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShowCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class USkeletalMesh*> Characters;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* VisibleCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
