// Fill out your copyright notice in the Description page of Project Settings.


#include "EndScreenWidget.h"
#include "Components/TextBlock.h"

void UEndScreenWidget::SetStats(int32 StageNumber, int32 Kills)
{
	if (StageReachedText)
	{
		StageReachedText->SetText(FText::Format(FText::FromString(TEXT("STAGE {0}")), StageNumber));
	}

	if (KillCountText)
	{
		KillCountText->SetText(FText::AsNumber(Kills));
	}
}
