#include "ProjectileBullet.h"
#include "Enemy.h"

AProjectileBullet::AProjectileBullet()
{
}

void AProjectileBullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;

	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
		Enemy->TakeDamageAmount(Damage);

	Destroy();
}