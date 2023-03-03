// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/PlayerState.h"
#include "CharacterMovement.h"

// Sets default values
AProjectile::AProjectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetSphereRadius(25.f);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);

	CollisionComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Function that initializes the projectile's velocity in the shoot direction.
void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//Apply Impulse
	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr && OtherComponent->IsSimulatingPhysics() && ProjectileMovementComponent)
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
}



void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ACharacterMovement* Target = Cast<ACharacterMovement>(OtherActor);
	ACharacterMovement* Shooter = GetInstigator<ACharacterMovement>();

	if (Target && Target != Shooter)
	{

		if (Target->IsLocallyControlled())
		{
			Target->CharacterTakeDamage(damage);
			//Killed other player
			if (Target->getCharacterState() == ECharacterState::Dead)
			{
				UE_LOG(LogTemp, Warning, TEXT("Other Character Died"));

				if (ARealmsPlayerState* state = Shooter->GetPlayerState<ARealmsPlayerState>())
				{
					UE_LOG(LogTemp, Warning, TEXT("Adding Kill..."));

					state->addKill();
				}
			}
		}

		if (HitSystem)
		{
			//UNiagaraFunctionLibrary
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitSystem, GetActorLocation());
		}

		this->Destroy();
	}
}