#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class ABaseProjectile;

UCLASS()
class VANGUARD_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

	UPROPERTY(EditAnywhere)
	float Weight = 0.0f;

protected:
	virtual void BeginPlay() override;
	virtual void Fire();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABaseProjectile> ProjectileToSpawn;

private:

};
