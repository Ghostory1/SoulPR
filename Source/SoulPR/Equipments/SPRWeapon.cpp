// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/SPRWeapon.h"

#include "Components/SPRCombatComponent.h"
#include "Data/SPRMontageActionData.h"

ASPRWeapon::ASPRWeapon()
{
}

void ASPRWeapon::EquipItem()
{
	Super::EquipItem();

	CombatComponent = GetOwner()->GetComponentByClass<USPRCombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;
		AttachToOwner(AttachSocket);
	}
}

void ASPRWeapon::UnequipItem()
{

}

UAnimMontage* ASPRWeapon::GetMontageForTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageActionData->GetMontageForTag(Tag, Index);
}
