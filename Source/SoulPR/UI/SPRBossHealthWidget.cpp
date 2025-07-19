// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPRBossHealthWidget.h"

#include "SPRStatBarWidget.h"
void USPRBossHealthWidget::Init(USPRAttributeComponent* AttributeComponent)
{
	if (AttributeComponent)
	{
		// ��������Ʈ ���
		AttributeComponent->OnAttributeChanged.AddUObject(this,&ThisClass::OnAttributeChanged);
		AttributeComponent->BroadcastAttributeChanged(ESPRAttributeType::Health);
	}
}

void USPRBossHealthWidget::OnAttributeChanged(ESPRAttributeType AttributeType, float InValue)
{
	if (AttributeType == ESPRAttributeType::Health)
	{
		HealthBarWidget->SetRatio(InValue);
	}
}