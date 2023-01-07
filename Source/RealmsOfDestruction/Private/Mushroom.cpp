// Fill out your copyright notice in the Description page of Project Settings.


#include "Mushroom.h"

AMushroom::AMushroom()
{
	//CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	//SetRootComponent(CollisionComponent);
	//CollisionComponent->SetSphereRadius(25.f);
	//CollisionComponent->OnComponentHit.AddDynamic(this, &AMushroom::OnHit);
	//CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
}

void AMushroom::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacterMovement* Target = Cast<ACharacterMovement>(OtherActor);
	ACharacterMovement* Shooter = GetInstigator<ACharacterMovement>();
	if (Target && Target != Shooter && Target->GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Player"));
		Target->CharacterTakeDamage(damage);
		this->Destroy();
	}
}