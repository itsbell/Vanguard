#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "WeaponGun.generated.h"

UCLASS()
class VANGUARD_API AWeaponGun : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	AWeaponGun();

	UPROPERTY(EditAnywhere)
	float FireInterval = 0.2f;

	float FireTimer = 0.0f;

	virtual void Tick(float DeltaTime) override;
};
