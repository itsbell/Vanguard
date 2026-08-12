#include "WeaponGatling.h"
#include "Projectiles/BaseProjectile.h"

AWeaponGatling::AWeaponGatling()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AWeaponGatling::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FireTimer += DeltaTime;
    if (FireTimer >= FireInterval)
    {
        FireTimer -= FireInterval;

        if (BarrelOffsets.Num() > 0)
        {
            FireFromBarrel(BarrelOffsets[CurrentBarrelIndex]);
            CurrentBarrelIndex = (CurrentBarrelIndex + 1) % BarrelOffsets.Num(); // 총열 순서대로 순환
        }
        else
        {
            FireFromBarrel(FVector::ZeroVector);
        }
    }
}

void AWeaponGatling::FireFromBarrel(const FVector& LocalOffset)
{
    if (!ProjectileToSpawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("ProjectileToSpawn is not set in Weapon class."));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();

    const FVector SpawnLocation = GetActorTransform().TransformPosition(LocalOffset);
    const FRotator SpawnRotation = GetActorRotation();

    GetWorld()->SpawnActor<ABaseProjectile>(ProjectileToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
}