#include "BaseWeapon.h"
#include "Projectiles/BaseProjectile.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FireInterval <= 0.0f)
		return;

	FireTimer += DeltaTime;
	if (FireTimer >= FireInterval)
	{
		Fire();
		FireTimer -= FireInterval;
	}
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::Fire()
{
	if (!ProjectileToSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileToSpawn is not set in Weapon class."));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	GetWorld()->SpawnActor<ABaseProjectile>(ProjectileToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);
}
