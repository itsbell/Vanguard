#include "WeaponLaser.h"
#include "Projectiles/ProjectileLaser.h"

AWeaponLaser::AWeaponLaser()
{
}

void AWeaponLaser::BeginPlay()
{
    Super::BeginPlay();

    Fire();
}