// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "CharacterMovement.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person camera component.
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetSphereRadius(25.f);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnHit);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacterMovement* Target = Cast<ACharacterMovement>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("powerup Out"));

	if (Target && Target->GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("powerup"));
		if (health)
		{
			Target->GainHealth(healthValue);
		}
		if (shield)
		{
			Target->GainShield(shieldValue);
		}
		if (speed)
		{
			Target->MultiplySpeed(speedValue);
		}

	}
}

