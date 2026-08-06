// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemy;
class AWeaponBox;

UCLASS()
class VANGUARD_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AEnemy* SpawnEnemy(TSubclassOf<AEnemy> EnemyClass);
	AWeaponBox* SpawnWeaponBox(TSubclassOf<AWeaponBox> AWeaponBoxClass);

private:
	UPROPERTY(EditAnywhere)
	float SpawnRange = 800.0f;

	UPROPERTY(EditAnywhere)
	float WeaponBoxOffset = 100.0f;
};
