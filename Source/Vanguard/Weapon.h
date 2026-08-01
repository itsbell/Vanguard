// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AProjectile;

UCLASS()
class VANGUARD_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(EditAnywhere)
	float Weight = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Fire();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileToSpawn;

	float FireTimer = 0.0f;

	UPROPERTY(EditAnywhere)
	float FireInterval = 0.2f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
