// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Damageable.h"
#include "Enemy.generated.h"

class AEnemy;
class AVanguardCharacter;
class UWidgetComponent;
class UHealthBarWidget;
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVanguardEnemyDied, AEnemy*, DeadEnemy);

UCLASS(meta = (PrioritizeCategories = "스탯 이동 몽타주"))
class VANGUARD_API AEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float Health = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "스탯")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "스탯")
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "스탯")
	float AttackRange = 250.0f; // 공격 사거리 (DistanceLimit <= AttackRange 이어야 함)

	UPROPERTY(EditAnywhere, Category = "스탯")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, Category = "스탯")
	float AttackSpeed = 2.0f; // 공격 간격(초). AttackMontage 길이보다 길어야 함 (쿨다운이 몽타주보다 짧으면 휘두르는 도중 Attack()이 또 호출되어 몽타주가 처음부터 재시작)

	UPROPERTY(EditAnywhere, Category = "이동")
	float DistanceStraight = 800.0f; // 캐릭터와의 Y축 거리가 이 값 미만이면 캐릭터 방향으로 추격 시작

	UPROPERTY(EditAnywhere, Category = "이동")
	float DistanceLimit = 200.0f; // 캐릭터와의 Y축 최소 유지 거리 (이 거리보다 더 가까이 접근하지 않음)

	UPROPERTY(EditAnywhere, Category = "몽타주")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, Category = "몽타주")
	TObjectPtr<UAnimMontage> AttackMontage;


	UPROPERTY()
	TObjectPtr<UHealthBarWidget> HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthBarWidgetComponent;

	bool bIsDead = false;

	float AttackTimer = 0.0f;

	FTimerHandle DeathTimerHandle;

	AVanguardCharacter* Character;

	FVector LaneForward = FVector::ZeroVector; // 스폰 시 전진 방향. 회전을 켜도 직진 축이 틀어지지 않게 고정해둔다
	
	void OnDeathFinished();
	void Attack();
    void FaceCharacter(float DeltaTime);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamageAmount(float DamageAmount) override;

	UPROPERTY(BlueprintAssignable)
	FOnVanguardEnemyDied OnEnemyDied;

	void OnAttackHit();
};
