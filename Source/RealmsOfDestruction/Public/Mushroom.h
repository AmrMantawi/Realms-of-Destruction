// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterMovement.h"
#include "Components/SphereComponent.h"
#include "Mushroom.generated.h"

/**
 * 
 */
UCLASS()
class AMushroom : public ACharacterMovement
{
	GENERATED_BODY()
public:
		AMushroom();

		// Sphere collision component.
		UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
			USphereComponent* CollisionComponent;

		UFUNCTION()
			void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
		UPROPERTY(EditAnywhere)
			float damage;
	
};
