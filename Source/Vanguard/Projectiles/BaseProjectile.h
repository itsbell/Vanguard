#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS(meta = (PrioritizeCategories = "스탯"))
class VANGUARD_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	ABaseProjectile();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "스탯")
	float Speed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "스탯")
	float Damage = 10.0f;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* ProjectileRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

};
