#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "WeaponGatling.generated.h"

UCLASS(meta = (PrioritizeCategories = "스탯"))
class VANGUARD_API AWeaponGatling : public ABaseWeapon
{
	GENERATED_BODY()

public:
	AWeaponGatling();

protected:
	UPROPERTY(EditAnywhere, Category = "스탯")
	TArray<FVector> BarrelOffsets;

	int32 CurrentBarrelIndex = 0;

	virtual void Fire() override;

	void FireFromBarrel(const FVector& LocalOffset);
};
