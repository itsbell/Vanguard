#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class ABaseProjectile;

UCLASS(meta = (PrioritizeCategories = "스탯"))
class VANGUARD_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();

	virtual void Tick(float DeltaTime) override;

	float GetWeight() const { return Weight; }

protected:
	virtual void BeginPlay() override;
	virtual void Fire();

	UPROPERTY(EditAnywhere, Category = "스탯")
	float Weight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "스탯")
	float FireInterval = 0.2f; // 자동 발사 간격(초). 0 이하면 자동 발사 안 함

	float FireTimer = 0.0f;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* WeaponRoot; // 소켓에 정확히 붙는 용도 (빈 컴포넌트, 오프셋 없음)

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseProjectile> ProjectileToSpawn;

private:

};
