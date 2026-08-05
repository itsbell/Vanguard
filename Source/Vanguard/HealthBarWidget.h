// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBar;

UCLASS()
class VANGUARD_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealthPercent(float Percent);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	bool bUseLowHealthColor = false;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float LowHealthThreshold = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	FLinearColor NormalColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	FLinearColor LowColor = FLinearColor(0.886f, 0.294f, 0.290f);
};
