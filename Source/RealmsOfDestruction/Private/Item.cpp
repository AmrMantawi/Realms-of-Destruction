// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "CharacterMovement.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create collision component and setup overlap event
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	CollisionComponent->SetSphereRadius(25.f);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlap);

	// Create mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(CollisionComponent);
	
	bReplicates = true;
}

void AItem::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AItem, ItemState);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	ItemState = EItemState::Active;
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnRep_HandleItemState()
{
	if (ItemState == EItemState::Active)
	{
		// Handle on state change to active
		Activate();
	}
	else
	{
		// Handle on state change to inactive
		Deactivate();
	}
}

void AItem::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacterMovement* Target = Cast<ACharacterMovement>(OtherActor);

	if (Target && Target->GetLocalRole() == ROLE_Authority && ItemState == EItemState::Active)
	{
		//Apply item effects
		if (bHealthBoost)
		{
			Target->GainHealth(HealthValue);
		}
		if (bShieldBoost)
		{
			Target->GainShield(ShieldValue);
		}
		if (bSpeedBoost)
		{
			Target->MultiplySpeed(SpeedValue);
		}
		if (bStrengthBoost)
		{
			Target->MultiplyDamage(StrengthValue);
		}

		//Deactivate item
		Deactivate();
	}
}


void AItem::Activate()
{
	//Set mesh to visible
	MeshComponent->SetVisibility(true);

	if (HasAuthority())
	{
		//Set item state to active
		ItemState = EItemState::Active;
	}
}

void AItem::Deactivate()
{
	//Set mesh to invisible
	MeshComponent->SetVisibility(false);

	if (NiagraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagraSystem, GetActorLocation());
	}

	if (HasAuthority())
	{
		//Set item state to Inactive
		ItemState = EItemState::Inactive;

		//Activate timer
		GetWorld()->GetTimerManager().SetTimer(PickupRespawnTimer, this, &AItem::Activate, RespawnTime, false);
	}
}