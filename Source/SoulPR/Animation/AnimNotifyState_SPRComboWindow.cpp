// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_SPRComboWindow.h"
#include "AnimNotifyState_SPRComboWindow.h"
#include "Character/SPRCharacter.h"

UAnimNotifyState_SPRComboWindow::UAnimNotifyState_SPRComboWindow(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotifyState_SPRComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ASPRCharacter* Character = Cast<ASPRCharacter>(MeshComp->GetOwner()))
	{
		Character->EnableComboWindow();
	}
}

void UAnimNotifyState_SPRComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ASPRCharacter* Character = Cast<ASPRCharacter>(MeshComp->GetOwner()))
	{
		Character->DisableComboWindow();
	}
}
