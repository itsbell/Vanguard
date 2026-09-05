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

void AWeaponLaser::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (ActiveLaser)
    {
        ActiveLaser->StopLaser();
        ActiveLaser = nullptr;
    }

    Super::EndPlay(EndPlayReason);
}

void AWeaponLaser::Fire()
{
    if (!ProjectileToSpawn || ActiveLaser) return;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();

    ActiveLaser = GetWorld()->SpawnActor<AProjectileLaser>(ProjectileToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);

    if (ActiveLaser)
        ActiveLaser->SetMuzzleTransform(MeshComponent, TEXT("MuzzleSocket"));
}
