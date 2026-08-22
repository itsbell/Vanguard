// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageBannerWidget.generated.h"

class UTextBlock;
class UBorder;

UCLASS()
class VANGUARD_API UStageBannerWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BannerText;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* LineTop;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* LineBottom;

public:
	void SetBanner(const FText& Label, const FLinearColor& TextColor, const FLinearColor& LineColor);
};
