// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SPREquipWeapon.h"
#include "SPRGameplayTags.h"
#include "Components/SPRCombatComponent.h"
#include "Equipments/SPRWeapon.h"

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

				// AttachToPlayer 함수보다 먼저 호출해야한다.
				CombatComponent->SetCombatEnabled(bCombatEnabled);
				MainWeapon->AttachToOwner(WeaponSocketName);
			}
		}
	}
}
