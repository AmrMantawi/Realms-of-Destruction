// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffPotato.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"


ABuffPotato::ABuffPotato()
{
	bReplicates = true;

	//Create collision components for right and left hand
	RightHand = CreateDefaultSubobject<UBoxComponent>(TEXT("RightPunchCollision"));
	RightHand->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	RightHand->SetupAttachment(GetMesh(), FName("hand_R"));
	RightHand->OnComponentHit.AddDynamic(this, &ABuffPotato::OnHit);
	RightHand->OnComponentBeginOverlap.AddDynamic(this, &ABuffPotato::OnOverlap);

	LeftHand = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftPunchCollision"));
	LeftHand->SetCollisionProfileName(FName("OverlapOnlyPawn"));
	LeftHand->SetupAttachment(GetMesh(),  FName("hand_L"));
	LeftHand->OnComponentHit.AddDynamic(this, &ABuffPotato::OnHit);
	LeftHand->OnComponentBeginOverlap.AddDynamic(this, &ABuffPotato::OnOverlap);
}

void ABuffPotato::BeginPlay()
{
	Super::BeginPlay();

}

void ABuffPotato::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABuffPotato, isPunching);
}

void ABuffPotato::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Compoenent hit"));
	//Return if player is not attempting to punch
	if (!isPunching) return;

	//Apply impulse to hit actor
	if (OtherActor != this && OtherComponent != nullptr && OtherComponent->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Warning, TEXT("Normal Impulse: %s"), *NormalImpulse.ToString());
		OtherComponent->AddImpulseAtLocation(NormalImpulse * 100.0f, Hit.ImpactPoint);
	}
}



void ABuffPotato::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Return if player is not attempting to punch
	if (!isPunching) return;

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
	}
}

void ABuffPotato::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABuffPotato::Server_StartPunch);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABuffPotato::Server_StopPunch);

}

void ABuffPotato::Server_StartPunch_Implementation()
{
	isPunching = true;
}

void ABuffPotato::Server_StopPunch_Implementation()
{
	isPunching = false;
}

