#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "ProjectileLaser.generated.h"

UCLASS(meta = (PrioritizeCategories = "스탯"))
class VANGUARD_API AProjectileLaser : public ABaseProjectile
{
	GENERATED_BODY()

public:
	AProjectileLaser();

	virtual void Tick(float DeltaTime) override;

	void SetMuzzleTransform(USceneComponent* InAttachComponent, FName InSocketName);
	void StopLaser();

protected:
	UPROPERTY(EditAnywhere, Category = "스탯")
	float MaxRange = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "스탯")
	float BeamThickness = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "스탯")
	float DamageInterval = 0.5f;

private:
	FTimerHandle DamageTimerHandle;

	UPROPERTY()
	TObjectPtr<AActor> CurrentHitActor;

	UPROPERTY()
	USceneComponent* MuzzleAttachComponent = nullptr;

	FName MuzzleSocketName = NAME_None;

	void UpdateBeam();
	void TickDamage();
};
