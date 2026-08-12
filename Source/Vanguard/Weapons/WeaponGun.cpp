#include "WeaponGun.h"

AWeaponGun::AWeaponGun()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AWeaponGun::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FireTimer += DeltaTime;
    if (FireTimer >= FireInterval)
    {
        Fire();
        FireTimer -= FireInterval;
    }
}