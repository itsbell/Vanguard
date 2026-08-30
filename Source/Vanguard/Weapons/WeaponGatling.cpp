#include "WeaponGatling.h"
#include "Projectiles/BaseProjectile.h"

AWeaponGatling::AWeaponGatling()
{
    FireInterval = 0.1f;
}

void AWeaponGatling::Fire()
{
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

void AWeaponGatling::FireFromBarrel(const FVector& LocalOffset)
{
    if (!ProjectileToSpawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("ProjectileToSpawn is not set in Weapon class."));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();

    FVector SpawnLocation = GetActorTransform().TransformPosition(LocalOffset);
    FRotator SpawnRotation = GetActorRotation();

    if (MeshComponent->DoesSocketExist(TEXT("MuzzleSocket")))
    {
        SpawnLocation = MeshComponent->GetSocketLocation(TEXT("MuzzleSocket"));
        SpawnRotation = MeshComponent->GetSocketRotation(TEXT("MuzzleSocket"));
    }

    GetWorld()->SpawnActor<ABaseProjectile>(ProjectileToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
}
