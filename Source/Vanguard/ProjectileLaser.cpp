#include "ProjectileLaser.h"
#include "Enemy.h"
#include "Damageable.h"

AProjectileLaser::AProjectileLaser()
{
    MeshComponent->OnComponentEndOverlap.AddDynamic(this, &AProjectileLaser::OnOverlapEnd);
}

void AProjectileLaser::Tick(float DeltaTime)
{
    FollowOwner();
}

void AProjectileLaser::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == GetOwner()) return;

    IDamageable* DamageableActor = Cast<IDamageable>(OtherActor);
    if (DamageableActor)
    {
        OverlappingTarget = OtherActor;

        DamageableActor->TakeDamageAmount(Damage);
        GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AProjectileLaser::TickDamage, DamageInterval, true);
        GetWorldTimerManager().SetTimer(LifetimeTimerHandle, this, &AProjectileLaser::ExpireLaser, Duration, false);
    }
}

void AProjectileLaser::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == OverlappingTarget)
    {
        GetWorldTimerManager().ClearTimer(DamageTimerHandle);
        OverlappingTarget = nullptr;
    }
}

void AProjectileLaser::TickDamage()
{
    if (OverlappingTarget)
    {
        AEnemy* Enemy = Cast<AEnemy>(OverlappingTarget);
        if (Enemy)
        {
            Enemy->TakeDamageAmount(Damage);
        }
    }
}

void AProjectileLaser::ExpireLaser()
{
    GetWorldTimerManager().ClearTimer(DamageTimerHandle);
    Destroy();
}

void AProjectileLaser::FollowOwner()
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor) return;

    FVector NewLocation;
    FRotator NewRotation = OwnerActor->GetActorRotation();

    // 무기/캐릭터에 소켓이 지정되어 있으면 그 소켓 위치 사용
    //if (MuzzleSocketName != NAME_None)
    //{
    //    if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor))
    //    {
    //        //if (OwnerCharacter->GetMesh()->DoesSocketExist(MuzzleSocketName))
    //        //{
    //        //    NewLocation = OwnerCharacter->GetMesh()->GetSocketLocation(MuzzleSocketName);
    //        //    NewRotation = OwnerCharacter->GetMesh()->GetSocketRotation(MuzzleSocketName);
    //        //}
    //        //else
    //        //{
    //        //    NewLocation = OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * MuzzleForwardOffset;
    //        //}
    //    }
    //}
    //else
    {
        // 소켓 없으면 단순히 전방으로 일정 거리 오프셋
        NewLocation = OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * MuzzleForwardOffset;
    }

    SetActorLocation(NewLocation);
    SetActorRotation(NewRotation);
}