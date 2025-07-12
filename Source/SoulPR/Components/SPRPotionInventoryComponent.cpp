// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SPRPotionInventoryComponent.h"
#include "SPRPotionInventoryComponent.h"

#include "Components/SPRAttributeComponent.h"
#include "Item/SPRPotion.h"
#include "GameFramework/Character.h"

USPRPotionInventoryComponent::USPRPotionInventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void USPRPotionInventoryComponent::DrinkPotion()
{
	if (PotionQuantity <= 0)
	{
		return;
	}
	
	if (USPRAttributeComponent* AttributeComponent = GetOwner()->GetComponentByClass< USPRAttributeComponent>())
	{
		// ���� �ϳ� ����
		PotionQuantity--;
		AttributeComponent->HealPlayer(PotionHealAmount);
	}
}

void USPRPotionInventoryComponent::SpawnPotion()
{
	PotionActor = GetWorld()->SpawnActor<ASPRPotion>(PotionClass, GetOwner()->GetActorTransform());
	if (PotionActor)
	{
		//���� �Ϸ� �� �տ� �ٿ���
		if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			PotionActor->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), PotionSocketName);
		}
	}
}

void USPRPotionInventoryComponent::DespawnPotion()
{
	if (IsValid(PotionActor))
	{
		PotionActor->Destroy();
	}
}

void USPRPotionInventoryComponent::SetPotionQuantity(int32 InQuantity)
{
	PotionQuantity = InQuantity;
}

