// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/SPRWeapon.h"
#include "Components/SPRCombatComponent.h"

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
		AttachToOwner(UnequipSocketName);
	}
}

void ASPRWeapon::UnequipItem()
{
}
