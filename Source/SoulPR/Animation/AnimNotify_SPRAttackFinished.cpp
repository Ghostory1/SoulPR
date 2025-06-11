// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SPRAttackFinished.h"
#include "Character/SPRCharacter.h"

UAnimNotify_SPRAttackFinished::UAnimNotify_SPRAttackFinished(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotify_SPRAttackFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ASPRCharacter* Character = Cast<ASPRCharacter>(MeshComp->GetOwner()))
	{
		Character->AttackFinished(ComboResetDelay);
	}
}
