// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_SPRParry.h"
#include "AnimNotifyState_SPRParry.h"

#include "Components/SPRStateComponent.h"

void UAnimNotifyState_SPRParry::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (USPRStateComponent* StateComponent = OwnerActor->GetComponentByClass<USPRStateComponent>())
		{
			StateComponent->SetState(SPRGameplayTags::Character_State_Parrying);
		}
	}
}

void UAnimNotifyState_SPRParry::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (USPRStateComponent* StateComponent = OwnerActor->GetComponentByClass<USPRStateComponent>())
		{
			StateComponent->ClearState();
		}
	}
}
