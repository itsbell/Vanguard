// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Damageable.h"
#include "WeaponBox.generated.h"

class ABaseWeapon;
class UHealthBarWidget;
class UWidgetComponent;

UCLASS()
class VANGUARD_API AWeaponBox : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;
	
	float Health = 100.0f;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY()
	TObjectPtr<UHealthBarWidget> HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthBarWidgetComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamageAmount(float DamageAmount) override;

};
