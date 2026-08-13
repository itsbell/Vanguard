#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "ProjectileGrenade.generated.h"

UCLASS(meta = (PrioritizeCategories = "스탯"))
class VANGUARD_API AProjectileGrenade : public ABaseProjectile
{
	GENERATED_BODY()

public:
	AProjectileGrenade();

protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, Category = "스탯")
	float SplashRadius = 300.0f;

	// 폭발 중심에서 멀어질수록 데미지를 줄일지 여부
	UPROPERTY(EditAnywhere, Category = "스탯")
	bool bUseDistanceFalloff = true;

private:
	void Explode();

};
