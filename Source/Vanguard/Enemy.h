// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

class AEnemy;
class AVanguardCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVanguardEnemyDied, AEnemy*, DeadEnemy);

UCLASS()
class VANGUARD_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float Health = 100.0f;

	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere)
	float DistanceStraight = 800.0f; // 캐릭터와의 Y축 거리가 이 값 미만이면 캐릭터 방향으로 추격 시작

	UPROPERTY(EditAnywhere)
	float DistanceLimit = 200.0f; // 캐릭터와의 Y축 최소 유지 거리 (이 거리보다 더 가까이 접근하지 않음)

	UPROPERTY(EditAnywhere)
	float AttackRange = 250.0f; // 공격 사거리 (DistanceLimit <= AttackRange 이어야 함)

	UPROPERTY(EditAnywhere)
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere)
	float AttackSpeed = 1.0f;
	
	float AttackTimer = 0.0f;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	AVanguardCharacter* Character;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamageAmount(float DamageAmount);

	UPROPERTY(BlueprintAssignable)
	FOnVanguardEnemyDied OnEnemyDied;

};
