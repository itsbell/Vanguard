#include "Weapon.h"
#include "Projectile.h"
// Sets default values
AWeapon::AWeapon()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Fire()
{
	if (!ProjectileToSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileToSpawn is not set in Weapon class."));
		return;
    }

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	GetWorld()->SpawnActor<AProjectile>(ProjectileToSpawn, GetActorLocation(), GetActorRotation(), SpawnParams);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FireTimer += DeltaTime;
	if (FireTimer >= FireInterval)
	{
		Fire();
		FireTimer = FireTimer - FireInterval;
	}
}

