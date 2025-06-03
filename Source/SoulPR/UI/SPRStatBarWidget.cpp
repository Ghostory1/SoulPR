// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPRStatBarWidget.h"

#include "Components/ProgressBar.h"

USPRStatBarWidget::USPRStatBarWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void USPRStatBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (StatBar)
	{
		StatBar->SetFillColorAndOpacity(FillColorAndOpacity);
	}
}

void USPRStatBarWidget::SetRatio(float Ratio) const
{
	if (StatBar)
	{
		StatBar->SetPercent(Ratio);
	}
}
