#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "WeaponGatling.generated.h"

UCLASS()
class VANGUARD_API AWeaponGatling : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	AWeaponGatling();

protected:
	UPROPERTY(EditAnywhere, Category = "Gatling")
	float FireInterval = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Gatling")
	TArray<FVector> BarrelOffsets;

	int32 CurrentBarrelIndex = 0;
	float FireTimer = 0.0f;

	virtual void Tick(float DeltaTime) override;

	void FireFromBarrel(const FVector& LocalOffset);
};
