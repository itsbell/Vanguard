#include "ProjectileGrenade.h"
#include "Damageable.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

AProjectileGrenade::AProjectileGrenade()
{
}

void AProjectileGrenade::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) return;

    Explode();
}

void AProjectileGrenade::Explode()
{
    const FVector ExplosionCenter = GetActorLocation();

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    GetWorld()->OverlapMultiByChannel(OverlapResults, ExplosionCenter, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SplashRadius),QueryParams);

    for (const FOverlapResult& Result : OverlapResults)
    {
        AActor* HitActor = Result.GetActor();
        if (!HitActor) continue;

        IDamageable* DamageableActor = Cast<IDamageable>(HitActor);
        if (!DamageableActor) continue;

        float FinalDamage = Damage;

        if (bUseDistanceFalloff)
        {
            const float Distance = FVector::Distance(ExplosionCenter, HitActor->GetActorLocation());
            const float FalloffRatio = FMath::Clamp(1.0f - (Distance / SplashRadius), 0.0f, 1.0f);
            FinalDamage = Damage * FalloffRatio;
        }

        if (FinalDamage > 0.0f)
        {
            DamageableActor->TakeDamageAmount(FinalDamage);
        }
    }

    Destroy();
}
