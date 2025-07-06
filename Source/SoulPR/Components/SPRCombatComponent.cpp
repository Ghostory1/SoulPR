// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SPRCombatComponent.h"
#include "Character/SPRCharacter.h"
#include "Equipments/SPRWeapon.h"
#include "Item/SPRPickupItem.h"
#include "Equipments/SPRShield.h"

USPRCombatComponent::USPRCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	
}



void USPRCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void USPRCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	
}

void USPRCombatComponent::SetWeapon(ASPRWeapon* NewWeapon)
{
	if (::IsValid(MainWeapon))
	{
		if (const AActor* OwnerActor = GetOwner())
		{	
			SpawnPickupItem(OwnerActor,MainWeapon->GetClass());

			MainWeapon->Destroy();
		}
	}
	MainWeapon = NewWeapon;
}

void USPRCombatComponent::SetArmour(ASPRArmour* NewArmour)
{
	const ESPRArmourType ArmourType = NewArmour->GetArmourType();
	
	// �̹� ���� ������ �����Ǿ������� �ٴڿ� PickupItem���� ������ NewArmour ����
	if (ASPRArmour* EquippedArmourPart = GetArmour(ArmourType))
	{
		if (IsValid(EquippedArmourPart))
		{
			// ������� ������ ���� �����Ǿ��ִ°Ŵ� PickupItem���� �������
			if (const AActor* OwnerActor = GetOwner())
			{
				SpawnPickupItem(OwnerActor, EquippedArmourPart->GetClass());
			}

			// ���� ����
			EquippedArmourPart->UnequipItem();
			// ����
			EquippedArmourPart->Destroy();
		}

		// ���� �Էµ� ��� ����
		ArmourMap[ArmourType] = NewArmour;
	}
	else
	{
		// ������ ������ ��� ���� ��� Add
		ArmourMap.Add(ArmourType, NewArmour);
	}
	
}

void USPRCombatComponent::SetShield(ASPRShield* NewShield)
{
	if (IsValid(Shield))
	{
		if (const AActor* OwnerActor = GetOwner())
		{
			SpawnPickupItem(OwnerActor, Shield->GetClass());

			Shield->Destroy();
		}
	}

	Shield = NewShield;
}

void USPRCombatComponent::SetCombatEnabled(const bool bEnabled)
{
	bCombatEnabled = bEnabled;
	if (OnChangedCombat.IsBound())
	{
		OnChangedCombat.Broadcast(bCombatEnabled);
	}
}

void USPRCombatComponent::SpawnPickupItem(const AActor* OwnerActor, const TSubclassOf<ASPREquipment>& NewEquipmentClass) const
{
	ASPRPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<ASPRPickupItem>(ASPRPickupItem::StaticClass(), OwnerActor->GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	PickupItem->SetEquipmentClass(NewEquipmentClass);
	PickupItem->FinishSpawning(OwnerActor->GetActorTransform());
}

