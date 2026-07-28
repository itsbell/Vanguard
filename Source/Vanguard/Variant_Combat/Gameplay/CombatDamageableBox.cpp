// Copyright Epic Games, Inc. All Rights Reserved.


#include "CombatDamageableBox.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

ACombatDamageableBox::ACombatDamageableBox()
{
	PrimaryActorTick.bCanEverTick = false;

	// create the mesh
	RootComponent = MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// set the collision properties
	MeshComponent->SetCollisionProfileName(FName("BlockAllDynamic"));

	// enable physics
	MeshComponent->SetSimulatePhysics(true);

	// disable navigation relevance so boxes don't affect NavMesh generation
	MeshComponent->bNavigationRelevant = false;
}

void ACombatDamageableBox::RemoveFromLevel()
{
	// destroy this actor
	Destroy();
}

void ACombatDamageableBox::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the death timer
	GetWorld()->GetTimerManager().ClearTimer(DeathTimer);
}

void ACombatDamageableBox::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
	// only process damage if we still have HP
	if (CurrentHP > 0.0f)
	{
		// apply the damage
		CurrentHP -= Damage;

		// are we dead?
		if (CurrentHP <= 0.0f)
		{
			HandleDeath();
		}

		// apply a physics impulse to the box, ignoring its mass
		MeshComponent->AddImpulseAtLocation(DamageImpulse * MeshComponent->GetMass(), DamageLocation);

		// call the BP handler to play effects, etc.
		OnBoxDamaged(DamageLocation, DamageImpulse);
	}
}

void ACombatDamageableBox::HandleDeath()
{
	// change the collision object type to Visibility so we ignore most interactions but still retain physics collisions
	MeshComponent->SetCollisionObjectType(ECC_Visibility);

	// call the BP handler to play effects, etc.
	OnBoxDestroyed();

	// set up the death cleanup timer
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &ACombatDamageableBox::RemoveFromLevel, DeathDelayTime);
}

void ACombatDamageableBox::ApplyHealing(float Healing, AActor* Healer)
{
	// stub
}

void ACombatDamageableBox::NotifyDanger(const FVector& DangerLocation, AActor* DangerSource)
{
	// stub
}

