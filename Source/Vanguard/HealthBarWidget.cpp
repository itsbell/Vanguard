// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthBarWidget::SetHealthPercent(float Percent)
{
	if (!HealthBar) return;

	const float Clamped = FMath::Clamp(Percent, 0.f, 1.f);
	HealthBar->SetPercent(Clamped);

	if (bUseLowHealthColor)
	{
		HealthBar->SetFillColorAndOpacity(Clamped <= LowHealthThreshold ? LowColor : NormalColor);
	}
}
