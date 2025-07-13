// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPRPlayerHUDWidget.h"
#include "SPRDefine.h"
#include "SPRStatBarWidget.h"
#include "Components/SPRAttributeComponent.h"
#include "SPRPotionWidget.h"
#include "Components/SPRPotionInventoryComponent.h"

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
			// 초기값 설정
			Attribute->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChanged);
			Attribute->BroadcastAttributeChanged(ESPRAttributeType::Stamina);
			Attribute->BroadcastAttributeChanged(ESPRAttributeType::Health);
		}

		if (USPRPotionInventoryComponent* PotionInventoryComponent = OwningPawn->GetComponentByClass<USPRPotionInventoryComponent>())
		{
			PotionInventoryComponent->OnUpdatePotionAmount.AddUObject(this, &ThisClass::OnPotionQuantityChanged);
			PotionInventoryComponent->BroadcastPotionUpdate();
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
		HealthBarWidget->SetRatio(InValue);
		break;
	}
}

void USPRPlayerHUDWidget::OnPotionQuantityChanged(int32 InAmount)
{
	if (PotionWidget)
	{
		PotionWidget->SetPotionQuantity(InAmount);
	}
}
