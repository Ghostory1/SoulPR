// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/SPRShield.h"
#include "SPRShield.h"
#include "Components/SPRCombatComponent.h"
#include "Equipments/SPRWeapon.h"
void ASPRShield::EquipItem()
{
	if (const AActor* OwnerActor = GetOwner())
	{
		if (USPRCombatComponent* CombatComponent = OwnerActor->GetComponentByClass<USPRCombatComponent>())
		{
			CombatComponent->SetShield(this);

			FName AttachSocketName = UnequipSocketName;
			// ���� ������ ���� ���� ��ġ ����

			// ���Ⱑ �ִ��� üũ, ���� ������ġ�� ����
			const ASPRWeapon* MainWeapon = CombatComponent->GetMainWeapon();
			if (IsValid(MainWeapon))
			{
				const ECombatType CombatType = MainWeapon->GetCombatType();
				if (CombatType == ECombatType::SwordShield)
				{
					if (CombatComponent->IsCombatEnabled())
					{
						AttachSocketName = EquipSocketName;
					}
				}
			}

			AttachToOwner(AttachSocketName);
		}
	}
}
