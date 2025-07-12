// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SPRPotionDrink.h"

#include "Components/SPRPotionInventoryComponent.h"

void UAnimNotify_SPRPotionDrink::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (USPRPotionInventoryComponent* PotionInventory = OwnerActor->GetComponentByClass<USPRPotionInventoryComponent>())
		{
			PotionInventory->DrinkPotion();
		}
	}
}
