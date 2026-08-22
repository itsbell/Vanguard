// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndScreenWidget.generated.h"

class UTextBlock;

UCLASS()
class VANGUARD_API UEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillCountText;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* StageReachedText;

public:
	void SetStats(int32 StageNumber, int32 Kills);
};
