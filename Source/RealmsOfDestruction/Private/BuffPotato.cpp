// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffPotato.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"


ABuffPotato::ABuffPotato()
{
	bReplicates = true;

	RightHand = CreateDefaultSubobject<UBoxComponent>(TEXT("RightPunchCollision"));
	RightHand->SetCollisionProfileName(FName("Punch"));
	RightHand->SetupAttachment(GetMesh(), FName("hand_R"));

	LeftHand = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftPunchCollision"));
	LeftHand->SetCollisionProfileName(FName("Punch"));
	LeftHand->SetupAttachment(GetMesh(),  FName("hand_L"));
}

void ABuffPotato::BeginPlay()
{
	Super::BeginPlay();

	RightHand->OnComponentBeginOverlap.AddDynamic(this, &ABuffPotato::OnPunchOverlapBegin);

	LeftHand->OnComponentBeginOverlap.AddDynamic(this, &ABuffPotato::OnPunchOverlapBegin);

}

void ABuffPotato::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABuffPotato, isPunching);
}

void ABuffPotato::OnPunchOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Outside If Statement"));

	// Check if the other actor is something that can be punched
	if (OtherActor && OtherActor != this && OtherComp && isPunching)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Something"));

		// Apply damage or effects to the other actor
		// You can use code like this to apply a force to the other actor and knock it back
		OtherComp->AddImpulseAtLocation(GetActorForwardVector() * PunchForce, SweepResult.Location, SweepResult.BoneName);

		ACharacterMovement* Target = Cast<ACharacterMovement>(OtherActor);
		ACharacterMovement* Shooter = GetInstigator<ACharacterMovement>();
		if (Target && Target != Shooter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Player"));

			if (Target->IsLocallyControlled())
			{
				Target->CharacterTakeDamage(damage);
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

