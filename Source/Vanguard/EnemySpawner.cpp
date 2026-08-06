// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Enemy.h"
#include "WeaponBox.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

AEnemy* AEnemySpawner::SpawnEnemy(TSubclassOf<AEnemy> EnemyClass)
{
	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyClass is not set in AEnemySpawner class."));
		return nullptr;
	}

	FVector SpawnLocation = GetActorLocation() + GetActorRightVector() * FMath::RandRange(-SpawnRange, SpawnRange);

	return GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, GetActorRotation());
}

AWeaponBox* AEnemySpawner::SpawnWeaponBox(TSubclassOf<AWeaponBox> WeaponBoxClass)
{
	if (!WeaponBoxClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnWeaponBox called with null WeaponBoxClass."));
		return nullptr;
	}
	FVector SpawnLocation = GetActorLocation() + GetActorRightVector() * WeaponBoxOffset;

	return GetWorld()->SpawnActor<AWeaponBox>(WeaponBoxClass, SpawnLocation, GetActorRotation());
}
