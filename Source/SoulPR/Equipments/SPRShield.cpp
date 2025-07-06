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
			// 무기 종류에 따라 부착 위치 설정

			// 무기가 있는지 체크, 방패 소켓위치를 조정
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
