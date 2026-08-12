#include "WeaponGrenade.h"

AWeaponGrenade::AWeaponGrenade()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AWeaponGrenade::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FireTimer += DeltaTime;
    if (FireTimer >= FireInterval)
    {
        Fire();
        FireTimer -= FireInterval;
    }
}