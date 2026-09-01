#include "BaseProjectile.h"
#include "Damageable.h"

ABaseProjectile::ABaseProjectile()
{
	ProjectileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileRoot"));
	RootComponent = ProjectileRoot;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	MeshComponent->SetupAttachment(ProjectileRoot);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnOverlapBegin);
}

void ABaseProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;
	
	if (OtherActor->IsA<ABaseProjectile>()) return; // 다른 발사체와는 충돌하지 않음

	// Apply damage to the other actor if it implements the IDamageable interface
	IDamageable* DamageableActor = Cast<IDamageable>(OtherActor);
	if (DamageableActor)
	{
		DamageableActor->TakeDamageAmount(Damage);
	}
	// Destroy the projectile after hitting something
	Destroy();
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ActorLocation = GetActorLocation();
	FVector ActorForwardVector = GetActorForwardVector();

	SetActorLocation(ActorLocation + ActorForwardVector * Speed * DeltaTime);
}
