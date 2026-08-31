// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemy;
class AWeaponBox;
class ABaseWeapon;

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
	AWeaponBox* SpawnWeaponBox(TSubclassOf<AWeaponBox> WeaponBoxClass, TSubclassOf<ABaseWeapon> WeaponClass);

private:
	UPROPERTY(EditAnywhere)
	float SpawnRange = 800.0f;

	UPROPERTY(EditAnywhere)
	float WeaponBoxOffset = 100.0f;

	// 박스는 중력이 없어 스폰된 높이에 그대로 머문다. 스포너 높이를 기준으로 Z를 보정한다
	UPROPERTY(EditAnywhere)
	float WeaponBoxHeightOffset = 0.0f;
};
