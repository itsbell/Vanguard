#include "WeaponLaser.h"
#include "Projectiles/ProjectileLaser.h"

AWeaponLaser::AWeaponLaser()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWeaponLaser::BeginPlay()
{
    Super::BeginPlay();

    Fire();
}