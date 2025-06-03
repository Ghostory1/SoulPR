// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPRPlayerHUDWidget.h"
#include "SPRStatBarWidget.h"
#include "Components/SPRAttributeComponent.h"

USPRPlayerHUDWidget::USPRPlayerHUDWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void USPRPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (USPRAttributeComponent* Attribute = OwningPawn->GetComponentByClass<USPRAttributeComponent>())
		{
			Attribute->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChanged);
			Attribute->BroadcastAttributeChanged(ESPRAttributeType::Stamina);
		}
	}
}

void USPRPlayerHUDWidget::OnAttributeChanged(ESPRAttributeType AttributeType, float InValue)
{
	switch (AttributeType)
	{
	case ESPRAttributeType::Stamina:
		StaminaBarWidget->SetRatio(InValue);
		break;
	case ESPRAttributeType::Health:
		break;
	}
}
