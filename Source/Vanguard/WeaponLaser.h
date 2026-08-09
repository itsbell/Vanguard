#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "WeaponLaser.generated.h"

UCLASS()
class VANGUARD_API AWeaponLaser : public ABaseWeapon
{
    GENERATED_BODY()

    AWeaponLaser();

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    class AProjectileLaser* ActiveLaser = nullptr;

};