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

    //UPROPERTY(EditAnywhere, Category = "Laser")
    //FName MuzzleSocketName = NAME_None;

    UPROPERTY(EditAnywhere, Category = "Laser")
    float MuzzleForwardOffset = 100.0f;


protected:
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY(EditDefaultsOnly, Category = "Laser")
    float DamageInterval = 0.5f;

    UPROPERTY(EditDefaultsOnly, Category = "Laser")
    float Duration = 3.0f;

private:
    FTimerHandle DamageTimerHandle;
    FTimerHandle LifetimeTimerHandle;

    UPROPERTY()
    AActor* OverlappingTarget = nullptr;

    void TickDamage();
    void ExpireLaser();
    void FollowOwner();
};
