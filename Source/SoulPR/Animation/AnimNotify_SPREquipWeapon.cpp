// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SPREquipWeapon.h"
#include "SPRGameplayTags.h"
#include "Components/SPRCombatComponent.h"
#include "Equipments/SPRWeapon.h"
#include "Equipments/SPRShield.h"

UAnimNotify_SPREquipWeapon::UAnimNotify_SPREquipWeapon(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotify_SPREquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const AActor* Owner = MeshComp->GetOwner())
	{
		if (USPRCombatComponent* CombatComponent = Owner->GetComponentByClass<USPRCombatComponent>())
		{
			if (ASPRWeapon* MainWeapon = CombatComponent->GetMainWeapon())
			{
				//���� ���� ����� ������ ���
				if (EquipmentType == ESPREquipmentType::Weapon)
				{
					bool bCombatEnabled = CombatComponent->IsCombatEnabled();
					FName WeaponSocketName;

					if (MontageActionTag == SPRGameplayTags::Character_Action_Equip)
					{
						bCombatEnabled = true;
						WeaponSocketName = MainWeapon->GetEquipSocketName();
					}
					else if (MontageActionTag == SPRGameplayTags::Character_Action_Unequip)
					{
						bCombatEnabled = false;
						WeaponSocketName = MainWeapon->GetUnequipSocketName();
					}



					// AttachToPlayer �Լ����� ���� ȣ���ؾ��Ѵ�.
					CombatComponent->SetCombatEnabled(bCombatEnabled);
					MainWeapon->AttachToOwner(WeaponSocketName);

				}
				// ���� ���� ����� ������ ���
				if (EquipmentType == ESPREquipmentType::Shield)
				{
					//���Ⱑ �Ѽذ��̸� ���и� ���� / ����
					if (MainWeapon->GetCombatType() == ECombatType::SwordShield)
					{
						if (ASPRShield* Shield = CombatComponent->GetShield())
						{
							FName ShieldSocketName;
							if (MontageActionTag == SPRGameplayTags::Character_Action_Equip)
							{
								ShieldSocketName = Shield->GetEquipSocketName();
							}
							else if (MontageActionTag == SPRGameplayTags::Character_Action_Unequip)
							{
								ShieldSocketName = Shield->GetUnequipSocketName();
							}

							Shield->AttachToOwner(ShieldSocketName);
						}
					}
				}
				
			}
		}
	}
}
