#include "ProjectileLaser.h"
#include "Characters/Enemy.h"
#include "Damageable.h"

AProjectileLaser::AProjectileLaser()
{
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectileLaser::Tick(float DeltaTime)
{
    UpdateBeam();
}

void AProjectileLaser::SetMuzzleTransform(USceneComponent* InAttachComponent, FName InSocketName)
{
    MuzzleAttachComponent = InAttachComponent;
    MuzzleSocketName = InSocketName;
}

void AProjectileLaser::UpdateBeam()
{
    if (!MuzzleAttachComponent || !IsValid(MuzzleAttachComponent->GetOwner()))
    {
        StopLaser();
        return;
    }
    
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor) return;
    if (!MuzzleAttachComponent) return;

    FVector StartLocation;
    FVector ForwardDir;

    if (MuzzleSocketName != NAME_None && MuzzleAttachComponent->DoesSocketExist(MuzzleSocketName))
    {
        StartLocation = MuzzleAttachComponent->GetSocketLocation(MuzzleSocketName);
        ForwardDir = MuzzleAttachComponent->GetSocketRotation(MuzzleSocketName).Vector();
    }
    else
    {
        StartLocation = MuzzleAttachComponent->GetComponentLocation();
        ForwardDir = MuzzleAttachComponent->GetForwardVector();
    }

    const FVector TraceEnd = StartLocation + ForwardDir * MaxRange;

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.AddIgnoredActor(OwnerActor);

    const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, TraceEnd, ECC_Visibility, QueryParams);

    const float BeamLength = bHit ? FVector::Distance(StartLocation, HitResult.ImpactPoint) : MaxRange;

    SetActorLocation(StartLocation);
    SetActorRotation(ForwardDir.Rotation());

    const float BaseCylinderLength = 100.0f;
    MeshComponent->SetRelativeScale3D(FVector(
        BeamThickness / BaseCylinderLength,
        BeamThickness / BaseCylinderLength,
        BeamLength / BaseCylinderLength));

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
