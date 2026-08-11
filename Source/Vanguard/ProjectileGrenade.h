#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "ProjectileGrenade.generated.h"

UCLASS()
class VANGUARD_API AProjectileGrenade : public ABaseProjectile
{
	GENERATED_BODY()
	
public:
	AProjectileGrenade();

    UPROPERTY(EditAnywhere, Category = "Grenade")
    float SplashRadius = 300.0f;

    // 폭발 중심에서 멀어질수록 데미지를 줄일지 여부
    UPROPERTY(EditAnywhere, Category = "Grenade")
    bool bUseDistanceFalloff = true;

protected:
    virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
    void Explode();

};
