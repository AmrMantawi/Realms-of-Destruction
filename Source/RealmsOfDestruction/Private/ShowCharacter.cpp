// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowCharacter.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AShowCharacter::AShowCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisibleCharacter = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	check(VisibleCharacter);
}

// Called when the game starts or when spawned
void AShowCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (VisibleCharacter != nullptr && Characters.Num() >= 0)
	{
		VisibleCharacter->SetSkeletalMesh(Characters[FMath::RandRange(0, Characters.Num() - 1)]);
	}
	
}

// Called every frame
void AShowCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

