// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SPRCombatComponent.h"
#include "Character/SPRCharacter.h"
#include "Equipments/SPRWeapon.h"
#include "Item/SPRPickupItem.h"

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
		if (ASPRCharacter* OwnerCharacter = Cast<ASPRCharacter>(GetOwner()))
		{
			ASPRPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<ASPRPickupItem>(ASPRPickupItem::StaticClass(), OwnerCharacter->GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			PickupItem->SetEquipmentClass(MainWeapon->GetClass());
			PickupItem->FinishSpawning(GetOwner()->GetActorTransform());

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
				ASPRPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<ASPRPickupItem>(ASPRPickupItem::StaticClass(), OwnerActor->GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
				PickupItem->SetEquipmentClass(EquippedArmourPart->GetClass());
				PickupItem->FinishSpawning(OwnerActor->GetActorTransform());
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

void USPRCombatComponent::SetCombatEnabled(const bool bEnabled)
{
	bCombatEnabled = bEnabled;
	if (OnChangedCombat.IsBound())
	{
		OnChangedCombat.Broadcast(bCombatEnabled);
	}
}

