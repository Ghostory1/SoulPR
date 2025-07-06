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
	
	// 이미 같은 부위가 장착되어있으면 바닥에 PickupItem으로 떨구고 NewArmour 장착
	if (ASPRArmour* EquippedArmourPart = GetArmour(ArmourType))
	{
		if (IsValid(EquippedArmourPart))
		{
			// 여기까지 들어오면 방어구가 장착되어있는거니 PickupItem으로 만들어줌
			if (const AActor* OwnerActor = GetOwner())
			{
				SpawnPickupItem(OwnerActor, EquippedArmourPart->GetClass());
			}

			// 장착 해제
			EquippedArmourPart->UnequipItem();
			// 삭제
			EquippedArmourPart->Destroy();
		}

		// 새로 입력된 장비를 저장
		ArmourMap[ArmourType] = NewArmour;
	}
	else
	{
		// 기존에 장착된 장비가 없는 경우 Add
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

