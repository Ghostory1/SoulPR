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
	
	// 이미 같은 부위가 장착되어있으면 바닥에 PickupItem으로 떨구고 NewArmour 장착
	if (ASPRArmour* EquippedArmourPart = GetArmour(ArmourType))
	{
		if (IsValid(EquippedArmourPart))
		{
			// 여기까지 들어오면 방어구가 장착되어있는거니 PickupItem으로 만들어줌
			if (const AActor* OwnerActor = GetOwner())
			{
				ASPRPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<ASPRPickupItem>(ASPRPickupItem::StaticClass(), OwnerActor->GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
				PickupItem->SetEquipmentClass(EquippedArmourPart->GetClass());
				PickupItem->FinishSpawning(OwnerActor->GetActorTransform());
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

void USPRCombatComponent::SetCombatEnabled(const bool bEnabled)
{
	bCombatEnabled = bEnabled;
	if (OnChangedCombat.IsBound())
	{
		OnChangedCombat.Broadcast(bCombatEnabled);
	}
}

