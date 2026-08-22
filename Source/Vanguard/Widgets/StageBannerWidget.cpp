// Fill out your copyright notice in the Description page of Project Settings.


#include "StageBannerWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UStageBannerWidget::SetBanner(const FText& Label, const FLinearColor& TextColor, const FLinearColor& LineColor)
{
    if (BannerText)
    {
        BannerText->SetText(Label);
        BannerText->SetColorAndOpacity(TextColor);
    }

    if (LineTop)
    {
        LineTop->SetBrushColor(LineColor);
    }

    if (LineBottom)
    {
        LineBottom->SetBrushColor(LineColor);
    }
}
