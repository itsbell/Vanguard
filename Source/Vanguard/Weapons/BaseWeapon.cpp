#include "BaseWeapon.h"
#include "Projectiles/BaseProjectile.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	RootComponent = WeaponRoot;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(WeaponRoot); // Root의 자식으로 부착
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

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	//if (MeshComponent->DoesSocketExist(TEXT("MuzzleSocket")))
	//{
	//	SpawnLocation = MeshComponent->GetSocketLocation(TEXT("MuzzleSocket"));
	//	SpawnRotation = MeshComponent->GetSocketRotation(TEXT("MuzzleSocket"));
	//}

	GetWorld()->SpawnActor<ABaseProjectile>(ProjectileToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
}
