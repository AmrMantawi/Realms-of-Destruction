// Fill out your copyright notice in the Description page of Project Settings.


#include "TransitionPawn.h"
#include "Kismet/GameplayStatics.h"
#include "EOSGameInstance.h"



// Sets default values
ATransitionPawn::ATransitionPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATransitionPawn::BeginPlay()
{
	Super::BeginPlay();

	EOSInstance = GetGameInstance<UEOSGameInstance>();

	if (EOSInstance)
	{
		if(EOSInstance->GetLoginStatus())
		{
			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				PC->ClientTravel("/Game/Maps/TestMenu", TRAVEL_Absolute);
			}
		}
	}
}

// Called every frame
void ATransitionPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATransitionPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

