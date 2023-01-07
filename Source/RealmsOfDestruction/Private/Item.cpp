// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "CharacterMovement.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person camera component.
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	MeshComponent->SetupAttachment(CollisionComponent);
	CollisionComponent->SetSphereRadius(25.f);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnHit);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	

}

void AItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AItem, itemState);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	itemState = EItemState::Active;
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnRep_HandleItemState()
{
	if (itemState == EItemState::Active)
	{
		Activate();
	}
	else if (itemState == EItemState::Inactive)
	{
		Deactivate();
	}
}

void AItem::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacterMovement* Target = Cast<ACharacterMovement>(OtherActor);
	//UE_LOG(LogTemp, Warning, TEXT("powerup Out"));

	if (Target && Target->GetLocalRole() == ROLE_Authority && itemState == EItemState::Active)
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
		if (strength)
		{
			Target->MultiplyDamage(strengthValue);
		}

		Deactivate();
	}
}


void AItem::Activate()
{
	if (HasAuthority())
	{
		itemState = EItemState::Active;
	}

	
}

void AItem::Deactivate()
{
	if (HasAuthority())
	{
		itemState = EItemState::Inactive;

		//Activate timer
	}
}


