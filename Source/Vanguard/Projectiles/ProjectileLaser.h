#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "ProjectileLaser.generated.h"

UCLASS()
class VANGUARD_API AProjectileLaser : public ABaseProjectile
{
	GENERATED_BODY()

public:
	AProjectileLaser();

    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Laser")
    float MaxRange = 3000.0f;

    UPROPERTY(EditAnywhere, Category = "Laser")
    float BeamThickness = 20.0f;

    void StopLaser();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Laser")
    float DamageInterval = 0.5f;

private:
    FTimerHandle DamageTimerHandle;

    UPROPERTY()
    AActor* CurrentHitActor = nullptr;

    void UpdateBeam();
    void TickDamage();
};
