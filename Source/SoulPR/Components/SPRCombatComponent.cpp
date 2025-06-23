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

void USPRCombatComponent::SetCombatEnabled(const bool bEnabled)
{
	bCombatEnabled = bEnabled;
	if (OnChangedCombat.IsBound())
	{
		OnChangedCombat.Broadcast(bCombatEnabled);
	}
}

