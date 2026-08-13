#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "WeaponLaser.generated.h"

class AProjectileLaser;

UCLASS()
class VANGUARD_API AWeaponLaser : public ABaseWeapon
{
    GENERATED_BODY()

    AWeaponLaser();

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    TObjectPtr<AProjectileLaser> ActiveLaser;

};
