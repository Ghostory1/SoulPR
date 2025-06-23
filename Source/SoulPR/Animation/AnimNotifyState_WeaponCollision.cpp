// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_WeaponCollision.h"

#include "Interfaces/SPRCombatInterface.h"

UAnimNotifyState_WeaponCollision::UAnimNotifyState_WeaponCollision(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotifyState_WeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ISPRCombatInterface* CombatInterface = Cast<ISPRCombatInterface>(OwnerActor))
		{
			CombatInterface->ActivateWeaponCollision(CollisionType);
		}
	}
}

void UAnimNotifyState_WeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (ISPRCombatInterface* CombatInterface = Cast<ISPRCombatInterface>(OwnerActor))
		{
			CombatInterface->DeactivateWeaponCollision(CollisionType);
		}
	}
}
