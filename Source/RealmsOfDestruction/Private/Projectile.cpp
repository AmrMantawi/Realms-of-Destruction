// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/PlayerState.h"
#include "CharacterMovement.h"

// Sets default values
AProjectile::AProjectile()
{
	//Projectile Collision Component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetSphereRadius(25.f);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	//Projectile Movemenmt Component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = Speed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	InitialLifeSpan = 3.0f;

	// Create a first person camera component.
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh != nullptr);

	Mesh->SetupAttachment(CollisionComponent);

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
	//Apply impulse to hit actor
	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr && OtherComponent->IsSimulatingPhysics() && ProjectileMovementComponent)
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
}



void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	ACharacterMovement* Target = Cast<ACharacterMovement>(OtherActor);
	ACharacterMovement* Shooter = GetInstigator<ACharacterMovement>();

	//Check if valid target
	if (Target && Target != Shooter)
	{
		//Apply damage to target
		if (Target->HasAuthority())
		{
			if (ARealmsPlayerState* ShooterPS = Shooter->GetPlayerState<ARealmsPlayerState>())
			{
				if (ARealmsPlayerState* TargetPS = Target->GetPlayerState<ARealmsPlayerState>())
				{
					//Apply damage if shooter and target are from different teams or if one is not in a team
					if (ShooterPS->GetTeam() == ETeam::NoTeam || ShooterPS->GetTeam() != TargetPS->GetTeam())
					{
						Target->CharacterTakeDamage(damage);

						//Add kill to shooter if target player died
						if (Target->getCharacterState() == ECharacterState::Dead)
						{
							ShooterPS->AddKill();
						}
					}
				}
			}
		}

		//Create niagra system on collision
		if (HitSystem)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitSystem, GetActorLocation());
		}

		//Destroy projectile
		this->Destroy();
	}
}