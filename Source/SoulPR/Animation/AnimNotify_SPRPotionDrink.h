// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SPRPotionDrink.generated.h"

/**
 * 
 */
UCLASS(meta=(DisplayName="Potion Drink"))
class SOULPR_API UAnimNotify_SPRPotionDrink : public UAnimNotify
{
	GENERATED_BODY()
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
