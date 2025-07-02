// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_SPRRotateTo.h"
#include "AnimNotifyState_SPRRotateTo.h"

#include "Components/SPRRotationComponent.h"

void UAnimNotifyState_SPRRotateTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (USPRRotationComponent* RotationComponent = MeshComp->GetOwner()->GetComponentByClass<USPRRotationComponent>())
	{
		// RoatationComponent 는 Enemy 에만 있기때문에 플레이어는 영향을 받지 않음
		RotationComponent->ToggleShouldRotate(true);
	}
}

void UAnimNotifyState_SPRRotateTo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (USPRRotationComponent* RotationComponent = MeshComp->GetOwner()->GetComponentByClass<USPRRotationComponent>())
	{
		// RoatationComponent 는 Enemy 에만 있기때문에 플레이어는 영향을 받지 않음
		RotationComponent->ToggleShouldRotate(false);
	}
}
