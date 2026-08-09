// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Damageable.h"
#include "WeaponBox.generated.h"

class ABaseWeapon;
class AVanguardCharacter;
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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 살아있는 모든 박스 목록. BeginPlay에서 등록하고 EndPlay에서 제거한다.
	// TActorIterator는 에디터에서 월드의 전체 액터 배열을 복사하므로 매 프레임 쓰기엔 무겁다.
	static TArray<TWeakObjectPtr<AWeaponBox>> ActiveBoxes;

	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere)
	float DistanceLimit = 400.0f; // 캐릭터와의 Y축 최소 유지 거리 (이 거리보다 더 가까이 접근하지 않음)

	UPROPERTY(EditAnywhere)
	float BoxSpacing = 200.0f; // 앞에 멈춰 있는 박스와 유지할 Y축 간격

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

	AVanguardCharacter* Character;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamageAmount(float DamageAmount) override;

	// 해당 월드에 남아 있는 박스를 모두 파괴한다 (스테이지 클리어 시 호출)
	static void DestroyAll(const UWorld* World);

	void SetWeaponClass(TSubclassOf<ABaseWeapon> NewWeaponClass) { WeaponClass = NewWeaponClass; }

};
