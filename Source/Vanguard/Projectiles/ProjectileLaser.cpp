#include "ProjectileLaser.h"
#include "Characters/Enemy.h"
#include "Damageable.h"

AProjectileLaser::AProjectileLaser()
{
    // Overlap 이벤트 기반이 아니므로 콜리전은 트레이스만 반응하도록 설정
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectileLaser::Tick(float DeltaTime)
{
    UpdateBeam();
}

void AProjectileLaser::UpdateBeam()
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor) return;

    const FVector StartLocation = OwnerActor->GetActorLocation();
    const FVector ForwardDir = OwnerActor->GetActorForwardVector();
    const FVector TraceEnd = StartLocation + ForwardDir * MaxRange;

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(OwnerActor);

    const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, TraceEnd, ECC_Pawn, QueryParams);

    const float BeamLength = bHit ? FVector::Distance(StartLocation, HitResult.ImpactPoint) : MaxRange;

    // 큐브(중심 피벗) 기준 - 시작점에서 절반만큼 앞으로 위치를 밀어줌
    const FVector CenterLocation = StartLocation + ForwardDir * (BeamLength * 0.5f);

    SetActorLocation(CenterLocation);
    SetActorRotation(ForwardDir.Rotation());

    // 기본 큐브는 한 변이 100 unit이라고 가정
    const float BaseCubeLength = 100.0f;
    MeshComponent->SetRelativeScale3D(FVector(
        BeamLength / BaseCubeLength,
        BeamThickness / BaseCubeLength,
        BeamThickness / BaseCubeLength));

    // 맞은 대상 처리 (대상이 바뀌었을 때만 갱신)
    AActor* NewHitActor = bHit ? HitResult.GetActor() : nullptr;

    if (NewHitActor != CurrentHitActor)
    {
        GetWorldTimerManager().ClearTimer(DamageTimerHandle);
        CurrentHitActor = NewHitActor;

        IDamageable* DamageableActor = CurrentHitActor ? Cast<IDamageable>(CurrentHitActor) : nullptr;
        if (DamageableActor)
        {
            DamageableActor->TakeDamageAmount(Damage);
            GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AProjectileLaser::TickDamage, DamageInterval, true);
        }
    }
}

void AProjectileLaser::TickDamage()
{
    IDamageable* DamageableActor = CurrentHitActor ? Cast<IDamageable>(CurrentHitActor) : nullptr;
    if (DamageableActor)
        DamageableActor->TakeDamageAmount(Damage);
}

void AProjectileLaser::StopLaser()
{
    GetWorldTimerManager().ClearTimer(DamageTimerHandle);
    Destroy();
}
