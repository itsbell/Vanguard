#include "WeaponLaser.h"
#include "ProjectileLaser.h"

AWeaponLaser::AWeaponLaser()
{
}

void AWeaponLaser::BeginPlay()
{
    Super::BeginPlay();

    Fire();
}