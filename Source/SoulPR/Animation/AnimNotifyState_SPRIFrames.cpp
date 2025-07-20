// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_SPRIFrames.h"
#include "AnimNotifyState_SPRIFrames.h"

#include "Interfaces/SPRCombatInterface.h"
void UAnimNotifyState_SPRIFrames::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	if (ISPRCombatInterface* CombatInterface = Cast<ISPRCombatInterface>(OwnerActor))
	{
		CombatInterface->ToggleFrames(true);
	}
}

void UAnimNotifyState_SPRIFrames::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	if (ISPRCombatInterface* CombatInterface = Cast<ISPRCombatInterface>(OwnerActor))
	{
		CombatInterface->ToggleFrames(false);
	}

	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
