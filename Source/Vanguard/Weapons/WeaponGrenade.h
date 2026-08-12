#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "WeaponGrenade.generated.h"

UCLASS()
class VANGUARD_API AWeaponGrenade : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	AWeaponGrenade();

	UPROPERTY(EditAnywhere)
	float FireInterval = 1.0f;

	float FireTimer = 0.0f;

	virtual void Tick(float DeltaTime) override;
};
