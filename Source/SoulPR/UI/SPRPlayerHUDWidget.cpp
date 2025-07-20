// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPRPlayerHUDWidget.h"
#include "SPRDefine.h"
#include "SPRStatBarWidget.h"
#include "Components/SPRAttributeComponent.h"
#include "SPRPotionWidget.h"
#include "Components/SPRPotionInventoryComponent.h"
#include "Components/SPRCombatComponent.h"
#include "Equipments/SPRWeapon.h"
#include "Equipments/SPRShield.h"
#include "UI/SPRWeaponWidget.h"

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
			// �ʱⰪ ����
			Attribute->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChanged);
			Attribute->BroadcastAttributeChanged(ESPRAttributeType::Stamina);
			Attribute->BroadcastAttributeChanged(ESPRAttributeType::Health);
		}

		if (USPRPotionInventoryComponent* PotionInventoryComponent = OwningPawn->GetComponentByClass<USPRPotionInventoryComponent>())
		{
			PotionInventoryComponent->OnUpdatePotionAmount.AddUObject(this, &ThisClass::OnPotionQuantityChanged);
			PotionInventoryComponent->BroadcastPotionUpdate();
		}

		if (USPRCombatComponent* CombatComponent = OwningPawn->GetComponentByClass<USPRCombatComponent>())
		{
			CombatComponent->OnChangedWeapon.AddUObject(this, &ThisClass::OnWeaponChanged);
			CombatComponent->OnChangedWeapon.Broadcast();
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

void USPRPlayerHUDWidget::OnWeaponChanged()
{
	if (const APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (const USPRCombatComponent* CombatComponent = OwningPawn->GetComponentByClass<USPRCombatComponent>())
		{
			UTexture2D* WeaponIconTexture = BlankWeaponIcon;
			UTexture2D* ShieldIconTexture = BlankWeaponIcon;

			// ������ �������� ����ش�.
			if (const ASPRWeapon* MainWeapon = CombatComponent->GetMainWeapon())
			{
				if (MainWeapon->GetCombatType() != ECombatType::MeleeFists)
				{
					// ���� ������
					WeaponIconTexture = MainWeapon->GetEquipmentIcon();
				}
			}

			// ���� �������� ����ش�.
			if (const ASPRShield* Shield = CombatComponent->GetShield())
			{
				ShieldIconTexture = Shield->GetEquipmentIcon();
			}

			// ���� ������ ����
			if (::IsValid(WeaponWidget))
			{
				WeaponWidget->SetWeaponImage(WeaponIconTexture);
			}

			// ���� ������ ����
			if (::IsValid(WeaponWidget))
			{
				ShieldWidget->SetWeaponImage(ShieldIconTexture);
			}
		}
	}
}
