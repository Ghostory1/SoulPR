// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_SPRPotionSpawn.h"

#include "Components/SPRPotionInventoryComponent.h"

void UAnimNotifyState_SPRPotionSpawn::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (USPRPotionInventoryComponent* PotionInventory = OwnerActor->GetComponentByClass<USPRPotionInventoryComponent>())
		{
			PotionInventory->SpawnPotion();
		}
	}
}

void UAnimNotifyState_SPRPotionSpawn::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (USPRPotionInventoryComponent* PotionInventory = OwnerActor->GetComponentByClass<USPRPotionInventoryComponent>())
		{
			PotionInventory->DespawnPotion();
		}
	}
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
